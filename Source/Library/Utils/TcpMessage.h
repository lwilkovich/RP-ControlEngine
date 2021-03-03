#ifndef __TCPMESSAGE_H__
#define __TCPMESSAGE_H__

#include "../Utils/Buffer.h"

namespace Engine {
    enum MessageType { BINARY };
    struct TcpMessage {
        long messageSize;
        MessageType messageType;
        std::shared_ptr<Buffer> buffer;
        TcpMessage() { this->buffer = std::make_shared<Buffer>(); }
        TcpMessage(TcpMessage const &tm) {
            this->messageSize = tm.messageSize;
            this->messageType = tm.messageType;
            this->buffer = tm.buffer;
        }
        // copy assignment
        TcpMessage &operator=(TcpMessage const &tm) {
            this->messageSize = tm.messageSize;
            this->messageType = tm.messageType;
            this->buffer = tm.buffer;
            return *this;
        }
    };

    inline std::shared_ptr<TcpMessage> TcpMesageFromString(std::string s) {
        std::shared_ptr<TcpMessage> mes(new TcpMessage());
        mes->messageType = BINARY;

        mes->messageSize = s.length();
        mes->buffer->Allocate(s.length());
        auto c = mes->buffer->getBufferCursor();
        c.writeBytes((const int8_t *)s.data(), s.length());
        return mes;
    }
} // namespace Engine
#endif