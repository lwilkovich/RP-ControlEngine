#ifndef __TCPSOCKETCLIENT_H__
#define __TCPSOCKETCLIENT_H__

#include "../Utils/Config.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include "Buffer.h"
#include "TcpMessage.h"
#include <arpa/inet.h>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_PACKET_SIZE 64 * 1024

namespace Engine {
    namespace Network {
        enum CurrentReadingState { MESSAGE_SIZE, MESSAGE_TYPE, MESSAGE };

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

            std::string getTag() { return TAG; }

            int getConnectionStatus();
            void setConnectionStatus(int b) { connectionStatus = b; }

            int createSocket();

            void closeSocketUnused(int unused);
            int closeSocket();

            int buildInternalSocketAddress();

            int createConnection();
            int createConnection(int socketTimeout);
            int bindSocket();
            int listenSocket();
            int acceptSocket();

            int readData(TcpMessage *message);

            int ping();
            ssize_t sendData(std::shared_ptr<TcpMessage> message);

          private:
            ssize_t sendData(const void *buffer, int size);
        };
    } // namespace Network
} // namespace Engine
#endif
