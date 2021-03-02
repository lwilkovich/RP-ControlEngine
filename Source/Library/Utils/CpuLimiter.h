#ifndef __CPULIMITER_H__
#define __CPULIMITER_H__

#include "_Logger.h"
#include "_String.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>

#define _DEFAULT_CPU_USAGE 5.0

namespace Engine {
    class CpuLimiter {
      private:
        const std::string TAG = "CpuLimiter";
        std::string tagPrefix;
        float cpuPercentage;

        unsigned long long cpuStart;
        unsigned long long cpuStop;
        unsigned long long workflowCpuTime;

      public:
        CpuLimiter(int cpu, std::string prefix = "") {
            tagPrefix = prefix;
            if ((cpu < 0) or (cpu > 100)) {
                cpuPercentage = _DEFAULT_CPU_USAGE;
            } else {
                cpuPercentage = cpu;
            }
            cpuStart = getTotalThreadTimeOnCpu();
        }
        const std::string getTag() { return TAG; }
        const std::string getTagPrefix() { return tagPrefix; }
        const std::string getDesc() { return ""; }
        const unsigned long long getTotalThreadTimeOnCpu() {
            struct rusage usage;
            getrusage(RUSAGE_THREAD, &usage);
            return (usage.ru_utime.tv_sec * 1000000) + usage.ru_utime.tv_usec + (usage.ru_stime.tv_sec * 1000000) + usage.ru_stime.tv_usec;
        }
        void CalculateAndSleep(bool systemDebug = false) {
            if (cpuPercentage == 100) {
                return;
            }
            cpuStop = getTotalThreadTimeOnCpu();
            workflowCpuTime = cpuStop - cpuStart;
            if (workflowCpuTime == 0) {
                cpuStart = getTotalThreadTimeOnCpu();
                return;
            }

            double sleepTimeLeft = ((cpuPercentage / 100.0) * workflowCpuTime);
            sleepTimeLeft = (workflowCpuTime / sleepTimeLeft) * workflowCpuTime - workflowCpuTime;
            // std::cout << "Workflow Time: " << workflowCpuTime << std::endl;
            // std::cout << "Time To Equalize: " << sleepTimeLeft << std::endl;
            // std::cout << "Percentage On Cpu: " << workflowCpuTime / (sleepTimeLeft + workflowCpuTime) << std::endl;
            // std::cout << "sleepTimeLeft: " << std::fixed << (unsigned int)sleepTimeLeft << std::endl;
            if (systemDebug) {
                SYSTEM(getTagPrefix() + getTag(), getDesc(), stringbuilder() << "Timers: " << cpuStart << " | " << cpuStop);
                SYSTEM(getTagPrefix() + getTag(), getDesc(), stringbuilder() << "Workflow Time: " << workflowCpuTime);
                SYSTEM(getTagPrefix() + getTag(), getDesc(), stringbuilder() << "Time To Equalize: " << sleepTimeLeft);
                SYSTEM(getTagPrefix() + getTag(), getDesc(), stringbuilder() << "Percentage On Cpu: " << workflowCpuTime / (sleepTimeLeft + workflowCpuTime));
                SYSTEM(getTagPrefix() + getTag(), getDesc(), stringbuilder() << "sleepTimeLeft: " << (unsigned int)sleepTimeLeft);
            }
            usleep((unsigned int)sleepTimeLeft);
            cpuStart = getTotalThreadTimeOnCpu();
        }
    };
} // namespace Engine
#endif