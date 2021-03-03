// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "../../../Source/Library/Utils/Buffer.h"
#include "../../../Source/Library/Utils/TcpMessage.h"
#define ADDRESS "127.0.0.1"
#define PORT 7225 

using namespace Engine;

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, ADDRESS, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    long messageLength = 50;
    TcpMessage message;
    message.messageType = BINARY;
    message.messageSize = 5;
    message.buffer->Allocate(message.messageSize);
    auto c = message.buffer->getBufferCursor();
    int8_t y = 'h';
    c.writeByte(&y);
    c.writeByte(&y);
    c.writeByte(&y);
    c.writeByte(&y);
    c.writeByte(&y);
    send(sock, &message.messageSize, sizeof(message.messageSize), 0);
    send(sock, &message.messageType, sizeof(message.messageType), 0);
    send(sock, c.getCursorData(), sizeof(message.messageSize), 0); 
    while (1) {
        continue;
    }
    return 0; 
} 