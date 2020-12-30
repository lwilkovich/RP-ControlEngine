#ifndef __LOGBATCHENTRY_H__
#define __LOGBATCHENTRY_H__

#include <mutex>

class LogThroughput {
  private:
    int systemCount;
    int infoCount;
    int debugCount;
    int warningCount;
    int errorCount;

    std::mutex systemPushLock;
    std::mutex infoPushLock;
    std::mutex debugPushLock;
    std::mutex warningPushLock;
    std::mutex errorPushLock;

    int systemMemSize;
    int infoMemSize;
    int debugMemSize;
    int warningMemSize;
    int errorMemSize;

    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> finishTime;

  public:
    LogThroughput()
    {
        startTime = std::chrono::system_clock::now();
        systemCount = 0;
        infoCount = 0;
        debugCount = 0;
        warningCount = 0;
        errorCount = 0;

        systemMemSize = 0;
        infoMemSize = 0;
        debugMemSize = 0;
        warningMemSize = 0;
        errorMemSize = 0;
    }

    LogThroughput(const LogThroughput &other)
    {
        systemCount = other.systemCount;
        infoCount = other.infoCount;
        debugCount = other.debugCount;
        warningCount = other.warningCount;
        errorCount = other.errorCount;

        systemMemSize = other.systemMemSize;
        infoMemSize = other.infoMemSize;
        debugMemSize = other.debugMemSize;
        warningMemSize = other.warningMemSize;
        errorMemSize = other.errorMemSize;

        startTime = other.startTime;
        finishTime = other.finishTime;
    }

    LogThroughput &operator=(const LogThroughput &other)
    {
        if (this != &other) {
            systemCount = other.systemCount;
            infoCount = other.infoCount;
            debugCount = other.debugCount;
            warningCount = other.warningCount;
            errorCount = other.errorCount;

            systemMemSize = other.systemMemSize;
            infoMemSize = other.infoMemSize;
            debugMemSize = other.debugMemSize;
            warningMemSize = other.warningMemSize;
            errorMemSize = other.errorMemSize;

            startTime = other.startTime;
            finishTime = other.finishTime;
        }
        return *this;
    }

    void close(std::chrono::time_point<std::chrono::system_clock> finish) { finishTime = finish; }

    auto getQTime() { return startTime; }

    int getSystemCount() { return systemCount; }

    int getInfoCount() { return infoCount; }

    int getDebugCount() { return debugCount; }

    int getWarningCount() { return warningCount; }

    int getErrorCount() { return errorCount; }

    int getTotalCount() { return getSystemCount() + getInfoCount() + getDebugCount() + getWarningCount() + getErrorCount(); }

    void pushSysLog(std::string logRecord)
    {
        std::lock_guard<std::mutex> guard(systemPushLock);
        systemCount += 1;
        systemMemSize += logRecord.size();
    }

    void pushInfoLog(std::string logRecord)
    {
        std::lock_guard<std::mutex> guard(infoPushLock);
        infoCount += 1;
        infoMemSize += logRecord.size();
    }

    void pushDebugLog(std::string logRecord)
    {
        std::lock_guard<std::mutex> guard(debugPushLock);
        debugCount += 1;
        debugMemSize += logRecord.size();
    }

    void pushWarningLog(std::string logRecord)
    {
        std::lock_guard<std::mutex> guard(warningPushLock);
        warningCount += 1;
        warningMemSize += logRecord.size();
    }

    void pushErrorLog(std::string logRecord)
    {
        std::lock_guard<std::mutex> guard(errorPushLock);
        errorCount += 1;
        errorMemSize += logRecord.size();
    }

    int size()
    {
        int currentSize = infoMemSize;
        currentSize += systemMemSize;
        currentSize += debugMemSize;
        currentSize += warningMemSize;
        currentSize += errorMemSize;
        currentSize += sizeof(systemCount);
        currentSize += sizeof(infoCount);
        currentSize += sizeof(debugCount);
        currentSize += sizeof(warningCount);
        currentSize += sizeof(errorCount);
        return currentSize;
    }
};
#endif