#include "TcpSocketClient.h"

using namespace Engine;
using namespace Network;

int TcpSocketClient::getConnectionStatus() {
    return connectionStatus;
}

int TcpSocketClient::createSocket() {
    if ((socketObject = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Socket Creation Error");
        return 1;
    } else {
        FD_ZERO(&readSet);
        FD_SET(socketObject, &readSet);
        fcntl(socketObject, F_SETFL, fcntl(socketObject, F_GETFL) | O_NONBLOCK);
        SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } TCP Socket Created");
        return 0;
    }
}

int TcpSocketClient::bindSocket() {
    if (bind(socketObject, (struct sockaddr *)&internalSocketAddress, sizeof(internalSocketAddress)) != 0) {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Error Binding Socket");
        return 1;
    }
    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Successfully Bound Socket");
    return 0;
}

int TcpSocketClient::listenSocket() {
    if (listen(socketObject, 3) == -1) {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Error Listening On Socket");
        return 1;
    }
    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Listening On Socket");
    return 0;
}

int TcpSocketClient::acceptSocket() {
    int tempSocketObject;

    if ((tempSocketObject = accept(socketObject, (struct sockaddr *)&internalSocketAddress, (socklen_t *)&internalSocketAddress)) == -1) {
        SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Error Accepting Socket");
        return 1;
    }
    close(socketObject);
    socketObject = tempSocketObject;
    connectionStatus = true;

    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Socket Connection Created");
    return 0;
}

int TcpSocketClient::buildInternalSocketAddress() {
    int opt = 1;
    if (setsockopt(socketObject, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Failure Building Interal Socket Address");
        return 1;
    }
    internalSocketAddress.sin_family = AF_INET;
    internalSocketAddress.sin_addr.s_addr = INADDR_ANY;
    internalSocketAddress.sin_port = htons(hostPort);

    // if(inet_pton(AF_INET, hostAddress.c_str(), &internalSocketAddress.sin_addr)<=0)
    // {
    //     printf("\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Internal Socket Address Build Success");
    return 0;
}

int TcpSocketClient::createConnection() {
    if (connect(socketObject, (struct sockaddr *)&internalSocketAddress, sizeof(internalSocketAddress)) == 0) {
        connectionStatus = true;
        SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Connection Created");
        return 0;
    }
    connectionStatus = false;
    DEBUG(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Connection Failed");
    return 1;
}

int TcpSocketClient::ping() {
    std::string mes = "ping";
    return sendData(mes.c_str(), 4);
}

ssize_t TcpSocketClient::sendData(std::shared_ptr<TcpMessage> message) {
    if (connectionStatus == true) {
        sendMutex.lock();
        // int length = message->messageSize;
        // std::cout <<"TESTTEST: "<< message->messageSize << std::endl;
        ssize_t n = send(socketObject, &message->messageSize, sizeof(message->messageSize), 0);
        if (n == -1) {
            ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } SEND Failure: { " << n << " }");
            closeSocket();
            sendMutex.unlock();
            return -1;
        }
        n = send(socketObject, &message->messageType, sizeof(message->messageType), 0);
        if (n == -1) {
            ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } SEND Failure: {" << n << " }");
            closeSocket();
            sendMutex.unlock();
            return -1;
        }
        n = send(socketObject, message->buffer->getBufferCursor().getCursorData(), message->messageSize, 0);
        if (n == -1) {
            ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } SEND Failure: {" << n << " }");
            closeSocket();
            sendMutex.unlock();
            return -1;
        }
        SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } SEND Message Size: { " << n << " }");
        sendMutex.unlock();
        return n;
    } else {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Not Connected");
        sendMutex.unlock();
        return -1;
    }
}

ssize_t TcpSocketClient::sendData(const void *buffer, int size) {
    if (connectionStatus == true) {
        ssize_t n = send(socketObject, buffer, size, 0);
        if (n == -1) {
            ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Send Failure");
            closeSocket();
            return -1;
        }
        SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Sent Message Size: { " << n << " }");
        return n;
    } else {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Not Connected");
        return -1;
    }
}

void TcpSocketClient::closeSocketUnused(int unused) {
    closeSocket();
    return;
}

int TcpSocketClient::closeSocket() {
    connectionStatus = false;
    if (close(socketObject) != 0) {
        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Failed To Close");
        return 1;
    }
    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Socket Closed");
    return 0;
}

int TcpSocketClient::readData(TcpMessage *message) {
    CurrentReadingState state = MESSAGE_SIZE;

    bool messageReadComplete = false;
    size_t readRemaining = sizeof(message->messageSize);
    if (connectionStatus == true) {
        while (messageReadComplete == false) {
            if (state == MESSAGE_SIZE) {
                while (readRemaining > 0) {
                    SYSTEM(getTag(), "", stringbuilder() << "Receiving Message Size...");
                    ssize_t n = recv(socketObject, &message->messageSize, sizeof(message->messageSize), 0);
                    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } READ MESSAGE_SIZE: { " << n << " }");
                    if (n == -1) {
                        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Read Failure");
                        closeSocket();
                        throw std::runtime_error("Failing Reading MESSAGE_SIZE");
                    }
                    readRemaining -= n;
                }
                readRemaining = sizeof(message->messageType);
                state = MESSAGE_TYPE;
            } else if (state == MESSAGE_TYPE) {
                while (readRemaining > 0) {
                    SYSTEM(getTag(), "", stringbuilder() << "Receiving Message Type...");
                    ssize_t n = recv(socketObject, &message->messageType, sizeof(message->messageType), 0);
                    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } READ MESSAGE_TYPE: { " << n << " }");
                    if (n == -1) {
                        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Read Failure");
                        closeSocket();
                        throw std::runtime_error("Failing Reading MESSAGE_TYPE");
                    }
                    readRemaining -= n;
                }
                readRemaining = message->messageSize;
                state = MESSAGE;
            } else if (state == MESSAGE) {
                message->buffer->Allocate(message->messageSize);
                BufferCursor cursor = message->buffer->getBufferCursor();
                while (readRemaining > 0) {
                    SYSTEM(getTag(), "", stringbuilder() << "Receiving Message Payload...");
                    ssize_t n = recv(socketObject, cursor.getCursorData(), readRemaining > MAX_PACKET_SIZE ? MAX_PACKET_SIZE : readRemaining, 0);
                    SYSTEM(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } READ MESSAGE: { " << n << " }");
                    if (n == -1) {
                        ERROR(getTag(), "", stringbuilder() << "Socket FD: { " << socketObject << " } Read Failure");
                        closeSocket();
                        throw std::runtime_error("Failing Reading MESSAGE");
                    }
                    cursor.jumpForward(n);
                    readRemaining -= n;
                }
                messageReadComplete = true;
            }
        }
    }
    return 0;
}