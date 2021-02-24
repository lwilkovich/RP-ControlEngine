#ifndef __NETWORKWRITERCONTROLLER_H__
#define __NETWORKWRITERCONTROLLER_H__

#include "../../Third-Party/json.hpp"
#include "../Static/NetworkInterface.h"
#include "../Utils/Config.h"
#include "../Utils/CpuLimiter.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include "Controller.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

typedef nlohmann::json json;

namespace Engine {
    namespace Network {
        class NetworkWriterController : public Controller {
          private:
            std::string TAG = "NetworkWriterController";

            // Config
            int cpuUsage;
            bool autoRestartFlag;

            int networkInterfaceIndex;

            NetworkInterface &networkInterface = NetworkInterface::getNetwork();

          public:
            NetworkWriterController(Config *config);
            ~NetworkWriterController() {}
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