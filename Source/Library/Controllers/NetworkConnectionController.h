#ifndef __NETWORKCONTROLLER_H__
#define __NETWORKCONTROLLER_H__

#include "Controller.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

class NetworkConnectionController : public Controller {
  private:
    std::string TAG = "NetworkConnectionController";

  public:
    NetworkConnectionController() {}
    ~NetworkConnectionController() {}
    const std::string &getTag() { return TAG; }
    std::string getDesc() { return "controllerData.classDescription"; }
    std::string getStatus() { return "hello"; }
    int getStatusInt() { return 1; }
    int startThread()
    {
        sleep(10);
        return 1;
    }
    bool getAutoRestartFlag() { return false; }
};
#endif