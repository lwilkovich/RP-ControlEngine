#include "NetworkWriterController.h"

using namespace Engine;
using namespace Network;

#define _CPU_USAGE "Cpu-Usage"
#define _AUTO_RESTART "Auto-Restart"

NetworkWriterController::NetworkWriterController(Config *config) {
    try {
        cpuUsage = config->settings[TAG][_CPU_USAGE];
        autoRestartFlag = config->settings[TAG][_AUTO_RESTART];

        INFO(getTag(), getDesc(), stringbuilder() << "Fetching Network Description...");
        networkInterfaceIndex = networkInterface.fetchNetworkDescriptionWriterController();
        INFO(getTag(), getDesc(), stringbuilder() << "Fetched Network Description: { " << networkInterfaceIndex << " }");
    } catch (const json::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        throw e;
    }
}

int NetworkWriterController::startThread() {
    try {
        CpuLimiter limiter(cpuUsage);
        while (1) {
            if (networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->getConnectionStatus() == true) {
                if (networkInterface.getNetworkList()[networkInterfaceIndex].writeBuffer.size() > 0) {
                    std::string mes = networkInterface.getNetworkList()[networkInterfaceIndex].writeBuffer.pop();
                    networkInterface.getNetworkList()[networkInterfaceIndex].socketClient->sendData(mes.c_str());
                }
            }
            limiter.CalculateAndSleep();
        }
    } catch (const std::exception &e) {
        ERROR(getTag(), getDesc(), e.what());
        return 1;
    }
    return 0;
}