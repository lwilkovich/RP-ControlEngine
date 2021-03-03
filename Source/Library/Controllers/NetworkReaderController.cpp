#include "NetworkReaderController.h"

using namespace Engine;
using namespace Network;

#define _CPU_USAGE "Cpu-Usage"
#define _AUTO_RESTART "Auto-Restart"

NetworkReaderController::NetworkReaderController(Config *config) {
    try {
        cpuUsage = config->settings[TAG][_CPU_USAGE];
        autoRestartFlag = config->settings[TAG][_AUTO_RESTART];

        INFO(getTag(), getDesc(), stringbuilder() << "Fetching Network Description...");
        networkInterfaceIndex = networkInterface.fetchNetworkDescriptionReaderController();
        INFO(getTag(), getDesc(), stringbuilder() << "Fetched Network Description: { " << networkInterfaceIndex << " }");
    } catch (const json::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        throw e;
    }
}

int NetworkReaderController::startThread() {
    try {
        CpuLimiter limiter(cpuUsage);
        while (1) {
            if (networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->getConnectionStatus() == true) {
                TcpMessage message;
                networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->readData(&message);
                SYSTEM(getTag(), getDesc(), stringbuilder() << "Message Received...");
            }
            limiter.CalculateAndSleep();
        }
    } catch (const std::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        return 1;
    }
    return 0;
}