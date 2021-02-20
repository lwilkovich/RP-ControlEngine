#ifndef __NETWORKINTERFACE_H__
#define __NETWORKINTERFACE_H__

#include <queue>
#include <string>
#include <memory>
#include "../Utils/TcpSocketClient.h"
#include "../Utils/_Queue.h"

namespace Engine {
    namespace Network {
        struct SocketDescription {
            std::string hostAddress;
            std::string description = "";
            int hostPort;
            int useTcpSock;
            int socketServerClientFlag;
            _Queue::Queue<std::string> writeBuffer;
            _Queue::Queue<std::string> readBuffer;
            std::shared_ptr<TcpSocketClient> socketClient;
            SocketDescription() {}
            SocketDescription(std::string hA, int hP, std::string desc = "", int sTF = 1, int sSCF = 1) {
                this->hostAddress = hA;
                this->hostPort = hP;
                this->useTcpSock = sTF;
                this->socketServerClientFlag = sSCF;
                this->description = desc;
                this->socketClient = std::make_shared<TcpSocketClient>(TcpSocketClient(this->hostAddress, this->hostPort));
            }
            void refresh() {
                this->socketClient = std::make_shared<TcpSocketClient>(TcpSocketClient(this->hostAddress, this->hostPort));
            }
        };

        class NetworkInterface {
            private:
                std::string TAG = "NetworkInterface";
            public:
                inline static const int socketCount = 4;
                SocketDescription networkList[socketCount] = {
                    SocketDescription("",7225, "Logs",1,1),
                    SocketDescription("",7226, "Sync",1,1),
                    SocketDescription("",7227, "Commands",1,1),
                    SocketDescription("",7228, "Camera",1,1)
                };
                NetworkInterface() {}
                std::string getTag() {
                    return TAG;
                }

                static NetworkInterface& getNetwork() {
                    static NetworkInterface n;
                    return n;
                }

                void setNetworkIndexAddress(std::string addr, int i) {
                    networkList[i].hostAddress = addr;
                    restartNetwork(i);
                }

                void setNetworkIndexPort(int port, int i) {
                    networkList[i].hostPort = port;
                    restartNetwork(i);
                }

                void restartNetwork(int i) {
                    networkList[i].socketClient->closeSocket();
                }
        };
    }
}
#endif