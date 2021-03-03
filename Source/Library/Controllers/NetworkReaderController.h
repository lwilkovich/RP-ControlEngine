#ifndef __NETWORKREADERCONTROLLER_H__
#define __NETWORKREADERCONTROLLER_H__

#include "../../Third-Party/json.hpp"
#include "../Static/NetworkInterface.h"
#include "../Utils/Buffer.h"
#include "../Utils/Config.h"
#include "../Utils/CpuLimiter.h"
#include "../Utils/TcpMessage.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include "Controller.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

typedef nlohmann::json json;

namespace Engine {
    namespace Network {
        class NetworkReaderController : public Controller {
          private:
            std::string TAG = "NetworkReaderController";

            // Config
            int cpuUsage;
            bool autoRestartFlag;

            int networkInterfaceIndex;

            NetworkInterface &networkInterface = NetworkInterface::getNetwork();

          public:
            NetworkReaderController(Config *config);
            ~NetworkReaderController() {}
            const std::string &getTag() { return TAG; }
            std::string getDesc() { return ""; }
            std::string getStatus() { return ""; }
            int getStatusInt() { return 1; }
            int startThread();

            // Config
            bool getAutoRestartFlag() { return autoRestartFlag; }
        };
    } // namespace Network
} // namespace Engine
#endif