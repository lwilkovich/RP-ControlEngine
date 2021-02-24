#ifndef __NETWORKCONNECTIONCONTROLLER_H__
#define __NETWORKCONNECTIONCONTROLLER_H__

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

        enum NetworkConnectionControllerState { TO_BUILD, TO_CONNECT, CONNECTED, DISCONNECTED };

        class NetworkConnectionController : public Controller {
          private:
            std::string TAG = "NetworkConnectionController";

            // Config
            int cpuUsage;
            bool autoRestartFlag;
            bool autoReconnectFlag;
            int networkInterfaceIndex;

            NetworkInterface &networkInterface = NetworkInterface::getNetwork();

          public:
            NetworkConnectionController(Config *config);
            ~NetworkConnectionController() {}
            const std::string &getTag() { return TAG; }
            std::string getDesc() { return ""; }
            std::string getStatus() { return ""; }
            int getStatusInt() { return 1; }
            int startThread();

            // Config
            bool getAutoRestartFlag() { return autoRestartFlag; }
            bool getAutoReconnectFlag() { return autoReconnectFlag; }
        };
    } // namespace Network
} // namespace Engine
#endif