#ifndef __LOGGINGCONTROLLER_H__
#define __LOGGINGCONTROLLER_H__

#include "../../Third-Party/json.hpp"
#include "../Static/LogQueue.h"
#include "../Utils/Config.h"
#include "../Utils/CpuLimiter.h"
#include "../Utils/LogThroughput.h"
#include "../Utils/_Bool.h"
#include "../Utils/_DateTime.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include "Controller.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

typedef nlohmann::json json;

namespace Engine {

    struct loggingFlags {
        bool info;
        bool debug;
        bool warning;
        bool system;
        bool error;
    };

    class LoggingController : public Controller {
      private:
        const std::string TAG = "LoggingController";
        _log::LogQueue &mLogger = _log::LogQueue::getInstance();

        std::ofstream fout;

        loggingFlags screen;
        loggingFlags network;
        loggingFlags file;
        int cpuUsage;

        float throughputInterval;

        std::string fileLocation;
        bool autoRestartFlag;

        typedef void (LoggingController::*MFP)(std::array<std::string, 4> holder, LogThroughput *);
        std::unordered_map<std::string, MFP> funcDict;

      public:
        LoggingController(Config *config);
        ~LoggingController() {}
        const std::string &getTag() { return TAG; }
        std::string getDesc() { return ""; }
        std::string getStatus() { return "Status"; }
        int getStatusInt() { return 1; }
        int startThread();
        bool getAutoRestartFlag() { return autoRestartFlag; }

        std::string buildLogRecord(std::array<std::string, 4> holder);
        json buildLogJson(std::array<std::string, 4> holder);

        void cutBatchEntry(LogThroughput *batchEntry, float elapsedTime);

        void logInfo(std::array<std::string, 4> holder, LogThroughput *batchEntry);
        void logSystem(std::array<std::string, 4> holder, LogThroughput *batchEntry);
        void logDebug(std::array<std::string, 4> holder, LogThroughput *batchEntry);
        void logWarning(std::array<std::string, 4> holder, LogThroughput *batchEntry);
        void logError(std::array<std::string, 4> holder, LogThroughput *batchEntry);

        void screenLogOut(std::string record);
        void screenLogErr(std::string record);
        void networkLog(std::string record);
        void fileLog(std::string record);
    };
} // namespace Engine
#endif