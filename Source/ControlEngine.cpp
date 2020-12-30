#include "Library/Controllers/LoggingController.h"
#include "Library/Controllers/NetworkConnectionController.h"
#include "Library/Controllers/ThreadOrchestrator.h"
#include "Library/Utils/Config.h"
#include "Library/Utils/CpuLimiter.h"
#include "Library/Utils/_Logger.h"
#include "Library/Utils/_String.h"
#include "Third-Party/json.hpp"
#include "limits.h"
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <unistd.h>

#define cliError " *** Failing reading the command line arguments ***"

typedef nlohmann::json json;

using namespace Engine;

#define _ENABLED "Enabled"
#define _THREAD_COUNT "Thread-Count"
#define _LOGGINGCONTROLLER "LoggingController"
#define _NETWORKCONNECTIONCONTROLLER "NetworkConnectionController"

std::vector<std::string> readCommandLineArgs(int argc, char *argv[])
{
    if (argc < 2) {
        throw cliError;
    }
    std::vector<std::string> parse = stringSplit(argv[1], "=");
    if ((parse.size() != 2) and (parse[0] == "--Config-File")) {
        throw cliError;
    }
    return parse;
}

int main(int argc, char *argv[])
{
    std::vector<std::string> cli = readCommandLineArgs(argc, argv);
    Config config;
    if (config.load(cli[1].c_str()) != 0) {
        return 1;
    }

    std::shared_ptr<ThreadOrchestrator> threadOrchestrator(new ThreadOrchestrator);
    
    if (config.settings[_LOGGINGCONTROLLER][_ENABLED]) {
        for (size_t i = 0; i < config.settings[_LOGGINGCONTROLLER][_THREAD_COUNT]; i++) {
            std::shared_ptr<LoggingController> lc(new LoggingController(&config));
            threadOrchestrator->pushControllerPool(lc);
        }        
    }
    if (config.settings[_NETWORKCONNECTIONCONTROLLER][_ENABLED]) {
        for (size_t i = 0; i < config.settings[_NETWORKCONNECTIONCONTROLLER][_THREAD_COUNT]; i++) {
            std::shared_ptr<NetworkConnectionController> ncc(new NetworkConnectionController);
            threadOrchestrator->pushControllerPool(ncc);
        }        
    }

    std::shared_ptr<ThreadContainer> threadOrchContainer(new ThreadContainer(threadOrchestrator));
    threadOrchContainer->startThread();

    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(UINT_MAX));
    }
}