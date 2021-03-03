#ifndef __COMMANDEXECUTERCONTROLLER_H__
#define __COMMANDEXECUTERCONTROLLER_H__

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
    class CommandExecuterController : public Controller {
      private:
        std::string TAG = "CommandExecuterController";

        // Config
        int cpuUsage;
        bool autoRestartFlag;

        int networkInterfaceIndex;

        Network::NetworkInterface &networkInterface = Network::NetworkInterface::getNetwork();

      public:
        CommandExecuterController(Config *config);
        ~CommandExecuterController() {}
        const std::string &getTag() { return TAG; }
        std::string getDesc() { return ""; }
        std::string getStatus() { return ""; }
        int getStatusInt() { return 1; }
        int startThread();

        // Config
        bool getAutoRestartFlag() { return autoRestartFlag; }
    };
} // namespace Engine
#endif