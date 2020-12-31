#ifndef __NETWORKCONNECTIONCONTROLLER_H__
#define __NETWORKCONNECTIONCONTROLLER_H__

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

  enum NetworkConnectionControllerState { TO_BUILD, TO_CONNECT, CONNECTED, DISCONNECTED };

  class NetworkConnectionController : public Controller {
    private:
      std::string TAG = "NetworkConnectionController";

      int cpuUsage;
      bool autoRestartFlag;

      NetworkInterface& networkInterface = NetworkInterface::getNetwork();
    public:
      NetworkConnectionController(Config *config);
      ~NetworkConnectionController() {}
      const std::string &getTag() { return TAG; }
      std::string getDesc() { return "controllerData.classDescription"; }
      std::string getStatus() { return "hello"; }
      int getStatusInt() { return 1; }
      int startThread();
      bool getAutoRestartFlag() { return false; }

      // Non-Controller Interface
      void connectionTCP();
      void connectionUDP();

      int listenConnection();
      void createConnection();
  };
}
}
#endif