#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include "../Utils/Config.h"
#include "../Utils/TcpSocketClient.h"
#include "../Utils/_Queue.h"
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

typedef nlohmann::json json;

#define _DESCRIPTION "Description"
#define _ADDRESS "Address"
#define _PORT "Port"
#define _PROTOCAL "Protocal"
#define _CONNECTION_TYPE "Connection-Type"

#define _FATAL_NETWORK_DESCRIPTION_FETCH " *** The NetworkInterface was unable to find a NetworkDescription for get() ***"

namespace Engine {
    namespace Network {
        struct NetworkDescription {
            std::string hostAddress;
            std::string description = "";
            int hostPort;
            int useTcpSock;
            int socketServerClientFlag;
            bool connectionControllerAttached;
            bool writerControllerAttached;
            bool readerControllerAttached;
            _Queue::Queue<std::string> writeBuffer;
            _Queue::Queue<std::string> readBuffer;
            std::shared_ptr<TcpSocketClient> socketClient;
            NetworkDescription() {}
            NetworkDescription(std::string hA, int hP, std::string desc = "", int sTF = 1, int sSCF = 1) {
                this->hostAddress = hA;
                this->hostPort = hP;
                this->useTcpSock = sTF;
                this->socketServerClientFlag = sSCF;
                this->description = desc;
                this->connectionControllerAttached = false;
                this->writerControllerAttached = false;
                this->readerControllerAttached = false;
                this->socketClient = std::make_shared<TcpSocketClient>(TcpSocketClient(this->hostAddress, this->hostPort));
            }
            void refresh() { this->socketClient = std::make_shared<TcpSocketClient>(TcpSocketClient(this->hostAddress, this->hostPort)); }
        };

        class NetworkInterface {
          private:
            std::string TAG = "NetworkInterface";
            std::mutex getDescriptionMutex;

          public:
            std::vector<NetworkDescription> networkList;
            NetworkInterface() {}
            std::string getTag() { return TAG; }
            int fetchNetworkDescriptionConnectionController() {
                getDescriptionMutex.lock();
                for (size_t i = 0; i < networkList.size(); i++) {
                    if (networkList[i].connectionControllerAttached == false) {
                        networkList[i].connectionControllerAttached = true;
                        getDescriptionMutex.unlock();
                        return i;
                    }
                }
                throw _FATAL_NETWORK_DESCRIPTION_FETCH;
                return -1;
            }
            int fetchNetworkDescriptionWriterController() {
                getDescriptionMutex.lock();
                for (size_t i = 0; i < networkList.size(); i++) {
                    if (networkList[i].writerControllerAttached == false) {
                        networkList[i].writerControllerAttached = true;
                        getDescriptionMutex.unlock();
                        return i;
                    }
                }
                throw _FATAL_NETWORK_DESCRIPTION_FETCH;
                return -1;
            }
            int fetchNetworkDescriptionReaderController() {
                getDescriptionMutex.lock();
                for (size_t i = 0; i < networkList.size(); i++) {
                    if (networkList[i].readerControllerAttached == false) {
                        networkList[i].readerControllerAttached = true;
                        getDescriptionMutex.unlock();
                        return i;
                    }
                }
                throw _FATAL_NETWORK_DESCRIPTION_FETCH;
                return -1;
            }

            std::vector<NetworkDescription> &getNetworkList() { return networkList; }

            static NetworkInterface &getNetwork() {
                static NetworkInterface n;
                return n;
            }

            int iniNetworkInterface(Config *config) {
                try {
                    size_t interfaceSize = config->settings[TAG].size();
                    INFO(getTag(), "", stringbuilder() << "Network Interface Size: { " << interfaceSize << " }");
                    for (size_t i = 0; i < interfaceSize; i++) {
                        NetworkDescription nd = NetworkDescription(
                            config->settings[TAG][i][_ADDRESS],
                            config->settings[TAG][i][_PORT],
                            config->settings[TAG][i][_DESCRIPTION],
                            (config->settings[TAG][i][_PROTOCAL] == "TCP") ? true : false,
                            (config->settings[TAG][i][_CONNECTION_TYPE] == "Server") ? true : false);
                        INFO(getTag(), "", stringbuilder() << "Network Interface { " << i << " }: " << config->settings[TAG][i].dump(-1, ' ', true));
                        networkList.push_back(nd);
                    }

                } catch (const std::exception &e) {
                    ERROR(getTag(), "", e.what());
                    return 1;
                }
                return 0;
            }

            void setNetworkIndexAddress(std::string addr, int i) {
                networkList[i].hostAddress = addr;
                restartNetwork(i);
            }

            void setNetworkIndexPort(int port, int i) {
                networkList[i].hostPort = port;
                restartNetwork(i);
            }

            void restartNetwork(int i) { networkList[i].socketClient->closeSocket(); }
        };
    } // namespace Network
} // namespace Engine
#endif