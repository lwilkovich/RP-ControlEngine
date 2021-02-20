#ifndef __NETWORKWRITERCONTROLLER_H__
#define __NETWORKWRITERCONTROLLER_H__

#include "../../Third-Party/json.hpp"
#include "Controller.h"
#include <iostream>
#include <time.h>
#include "../Utils/_Logger.h"
#include <unistd.h>
#include "../Utils/_String.h"
#include "../Utils/Config.h"
#include "../Utils/CpuLimiter.h"
#include "../Static/NetworkInterface.h"

typedef nlohmann::json json;

namespace Engine {
namespace Network {
  class NetworkWriterController : public Controller {
    private:
      std::string TAG = "NetworkWriterController";

      // Config
      int cpuUsage;
      bool autoRestartFlag;

      NetworkInterface& networkInterface = NetworkInterface::getNetwork();
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
}
}
#endif