#ifndef __TCPMESSAGE_H__
#define __TCPMESSAGE_H__

#include "../Utils/Buffer.h"

namespace Engine {
    enum MessageType { BINARY };
    struct TcpMessage {
        Buffer buffer;
        MessageType messageType;
        long messageSize;
    };
}
#endif