#ifndef __TCPSOCKETCLIENT_H__
#define __TCPSOCKETCLIENT_H__

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../Utils/Config.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include <iostream>
#include <signal.h>
#include <exception>
#include <mutex>

#define MAX_PACKET_SIZE 64 * 1024

namespace Engine {
namespace Network {
    enum CurrentReadingState { MESSAGE_SIZE, MESSAGE };

    class TcpSocketClient {
        private:
            const std::string TAG = "TcpSocketClient";

            std::string hostAddress = "";
            int hostPort;
            double socketTimeout = 0.5;

            struct sockaddr_in internalSocketAddress;
            int addrlen = sizeof(internalSocketAddress);

            struct sockaddr cliaddr;
            socklen_t cliaddrlen = sizeof(cliaddr);

            int mValRead;

            char mBuffer[1];
            bool connectionStatus = false;

            std::mutex sendMutex;

            fd_set readSet;
            struct timeval timeout = {1, 0};

            int socketObject = 0;
        public:
            TcpSocketClient(std::string addr, int port) {
                hostAddress = addr;
                hostPort = port;
                memset(&cliaddr, 0, sizeof(cliaddr));
            }
            TcpSocketClient() {}
            TcpSocketClient(const TcpSocketClient &tsc) {
                hostAddress = tsc.hostAddress;
                hostPort = tsc.hostPort;
            } 
            
            std::string getTag() {
                return TAG;
            }

            int getConnectionStatus();
            void setConnectionStatus(int b) {
                connectionStatus = b;
            }

            int createSocket();

            void closeSocketUnused(int unused);
            int closeSocket();

            int buildInternalSocketAddress();

            int createConnection();
            int createConnection(int socketTimeout);
            int bindSocket();
            int listenSocket();
            int acceptSocket();
            
            std::vector<int8_t> readData();

            int ping();
            ssize_t sendData(const char *message);
        private:
            ssize_t sendData(const void *buffer, int size);
    };
}
}
#endif
