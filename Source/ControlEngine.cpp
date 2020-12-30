#include <iostream>
#include "./Library/Utils/Config.h"
#include "./Library/Utils/_String.h"
#include "./Third-Party/json.hpp"
#include "limits.h"
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include "./Library/Utils/_Logger.h"
#include "./Library/Controllers/ThreadOrchestrator.h"
#include "./Library/Controllers/NetworkConnectionController.h"
#include "./Library/Controllers/LoggingController.h"
#include "./Library/Utils/CpuLimiter.h"

#define cliError " *** Failing reading the command line arguments ***"

typedef nlohmann::json json;

using namespace Engine;

std::vector<std::string> readCommandLineArgs(int argc, char *argv[]) {
    if (argc < 2) {
        throw cliError;
    }
    std::vector<std::string> parse = stringSplit(argv[1], "=");
    if ((parse.size() != 2) and (parse[0] == "--Config-File")) {
        throw cliError;
    }
    // ConfigReader cr(parse[1].c_str());

    return parse;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> cli = readCommandLineArgs(argc, argv);
    Config config;
    if (config.load(cli[1].c_str()) != 0) {
        return 1;
    }
    std::shared_ptr<NetworkConnectionController> ncc(new NetworkConnectionController);
    std::shared_ptr<LoggingController> lc(new LoggingController(&config));

    std::shared_ptr<ThreadOrchestrator> threadOrchestrator(new ThreadOrchestrator);
    threadOrchestrator->pushControllerPool(ncc);
    threadOrchestrator->pushControllerPool(lc);

    std::shared_ptr<ThreadContainer> threadOrchContainer(new ThreadContainer(threadOrchestrator));
    threadOrchContainer->startThread();

    while (1) {
        std::this_thread::sleep_for (std::chrono::seconds(UINT_MAX));
    }
}