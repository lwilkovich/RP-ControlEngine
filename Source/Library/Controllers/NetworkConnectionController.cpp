#include "NetworkConnectionController.h"

using namespace Engine;
using namespace Network;

#define _CPU_USAGE "Cpu-Usage"
#define _AUTO_RESTART "Auto-Restart"
#define _AUTO_RECONNECT "Auto-Reconnect"

NetworkConnectionController::NetworkConnectionController(Config *config) {
    try {
        cpuUsage = config->settings[TAG][_CPU_USAGE];
        autoRestartFlag = config->settings[TAG][_AUTO_RESTART];
        autoReconnectFlag = config->settings[TAG][_AUTO_RECONNECT];
    }
    catch (const json::exception& e) {
        ERROR(getTag(), getDesc(), e.what());
    }
}

int NetworkConnectionController::startThread() {
    NetworkConnectionControllerState state = TO_BUILD;
    try {
        CpuLimiter limiter(cpuUsage);
        while (1) {
            if ((networkInterface.networkList[0].socketClient->getConnectionStatus() == false) and (state == TO_BUILD)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Signaling Socket To Build");
                networkInterface.networkList[0].socketClient->createSocket();
                networkInterface.networkList[0].socketClient->buildInternalSocketAddress();
                networkInterface.networkList[0].socketClient->bindSocket();
                networkInterface.networkList[0].socketClient->listenSocket();
                state = TO_CONNECT;
            }
            if (state == TO_CONNECT) {
                if (networkInterface.networkList[0].socketClient->acceptSocket() == 0) {
                    INFO(getTag(), getDesc(), stringbuilder() << "Network Connection Successful");
                    state = CONNECTED;
                }
                else {
                    usleep(1000000 * 1);
                }
            }
            if ((networkInterface.networkList[0].socketClient->getConnectionStatus() == false) and (state == CONNECTED)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Network Disconnect Identified");
                state = DISCONNECTED;
            }
            if (state == CONNECTED) { 
                networkInterface.networkList[0].socketClient->ping();
            }
            if ((getAutoReconnectFlag()) and (state == DISCONNECTED)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Signaling Socket To Rebuild");
                state = TO_BUILD;
            } 
            limiter.CalculateAndSleep();
        }
    }
    catch (const std::exception& e) {
        ERROR(getTag(), getDesc(), e.what());
        return 1;
    }
    return 0;
}