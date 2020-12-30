#ifndef __CPULIMITER_H__
#define __CPULIMITER_H__

#include "_Logger.h"
#include "_String.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>
#include <unistd.h>

#define _DEFAULT_CPU_USAGE 20

namespace Engine {
    class CpuLimiter {
      private:
        const std::string TAG = "CpuLimiter";
        int cpuPercentage;
        std::clock_t cpuStart;
        std::chrono::time_point<std::chrono::high_resolution_clock> clockStart;

        std::clock_t cpuStop;
        std::chrono::time_point<std::chrono::high_resolution_clock> clockStop;

      public:
        CpuLimiter(int cpu)
        {
            if ((cpu < 0) or (cpu > 100)) {
                cpuPercentage = _DEFAULT_CPU_USAGE;
            }
            else {
                cpuPercentage = cpu;
            }
            clockStart = std::chrono::high_resolution_clock::now();
            cpuStart = std::clock();
        }
        const std::string getTag() { return TAG; }
        const std::string getDesc() { return "Temp"; }
        void CalculateAndSleep()
        {
            if (cpuPercentage == 100) {
                return;
            }
            cpuStop = std::clock();
            clockStop = std::chrono::high_resolution_clock::now();
            auto cpuMS = 1000.0 * (cpuStop - cpuStart) / CLOCKS_PER_SEC;
            if (cpuMS < 1) {
                cpuMS = 1;
            }
            auto clockMS = std::chrono::duration<double, std::milli>(clockStop - clockStart).count();
            if (clockMS < 1) {
                clockMS = 1;
            }
            if (cpuMS / float(clockMS) * 100 < cpuPercentage) {
                // std::cout << "No Need To Sleep" << std::endl;
                return;
            }
            double sleepTimeLeft = (cpuPercentage / 100.0) * clockMS;
            sleepTimeLeft = (cpuMS / float(sleepTimeLeft)) * clockMS - clockMS;
            // std::cout << "sleepTimeLeft: " << sleepTimeLeft << std::endl;
            // std::cout << cpuMS << " / " << float(clockMS+sleepTimeLeft) << std::endl;
            // std::cout << "Real Percentage: " << cpuMS/float(clockMS+sleepTimeLeft) << std::endl;
            // SYSTEM(getTag(), getDesc(),
            //     stringbuilder() << cpuMS << " / " << float(clockMS+sleepTimeLeft));
            // SYSTEM(getTag(), getDesc(),
            //     stringbuilder() << "Real Percentage: " << cpuMS/float(clockMS+sleepTimeLeft));

            std::this_thread::sleep_for(std::chrono::milliseconds(int64_t(sleepTimeLeft)));
            clockStart = std::chrono::high_resolution_clock::now();
            cpuStart = std::clock();
        }
    };
} // namespace Engine
#endif