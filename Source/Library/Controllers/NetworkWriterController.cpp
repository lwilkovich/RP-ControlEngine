#include "NetworkWriterController.h"

using namespace Engine;
using namespace Network;

#define _CPU_USAGE "Cpu-Usage"
#define _AUTO_RESTART "Auto-Restart"

NetworkWriterController::NetworkWriterController(Config *config) {
    try {
        cpuUsage = config->settings[TAG][_CPU_USAGE];
        autoRestartFlag = config->settings[TAG][_AUTO_RESTART];
    }
    catch (const json::exception& e) {
        ERROR(getTag(), getDesc(), e.what());
    }
}

int NetworkWriterController::startThread() {
    try {
        CpuLimiter limiter(cpuUsage);
        while (1) {
            limiter.CalculateAndSleep();
        }
    }
    catch (const std::exception& e) {
        ERROR(getTag(), getDesc(), e.what());
        return 1;
    }
    return 0;
}