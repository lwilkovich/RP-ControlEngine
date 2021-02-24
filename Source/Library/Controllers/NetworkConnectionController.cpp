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

        INFO(getTag(), getDesc(), stringbuilder() << "Fetching Network Description...");
        networkInterfaceIndex = networkInterface.fetchNetworkDescriptionConnectionController();
        INFO(getTag(), getDesc(), stringbuilder() << "Fetched Network Description: { " << networkInterfaceIndex << " }");
    } catch (const std::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        throw e;
    }
}

int NetworkConnectionController::startThread() {
    NetworkConnectionControllerState state = TO_BUILD;
    try {
        CpuLimiter limiter(cpuUsage);
        while (1) {
            if ((networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->getConnectionStatus() == false) and (state == TO_BUILD)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Signaling Socket To Build");
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->createSocket();
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->buildInternalSocketAddress();
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->bindSocket();
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->listenSocket();
                state = TO_CONNECT;
            }
            if (state == TO_CONNECT) {
                if (networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->acceptSocket() == 0) {
                    INFO(getTag(), getDesc(), stringbuilder() << "Network Connection Successful");
                    state = CONNECTED;
                } else {
                    usleep(1000000 * 1);
                }
            }
            if ((networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->getConnectionStatus() == false) and (state == CONNECTED)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Network Disconnect Identified");
                state = DISCONNECTED;
            }
            if (state == CONNECTED) {
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->ping();
                usleep(1000000 * 5);
            }
            if ((getAutoReconnectFlag()) and (state == DISCONNECTED)) {
                INFO(getTag(), getDesc(), stringbuilder() << "Signaling Socket To Rebuild");
                state = TO_BUILD;
            }
            limiter.CalculateAndSleep();
        }
    } catch (const std::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        return 1;
    }
    return 0;
}