#ifndef __ThreadOrchestrator_H__
#define __ThreadOrchestrator_H__

#include "../Utils/CpuLimiter.h"
#include "../Utils/ThreadContainer.h"
#include "../Utils/_Logger.h"
#include "../Utils/_String.h"
#include "Controller.h"
#include <iostream>
#include <memory>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <vector>

namespace Engine {
    class ThreadOrchestrator : public Controller {
      private:
        const std::string TAG = "ThreadOrchestrator";
        std::vector<std::shared_ptr<ThreadContainer>> threadContainerPool;
        std::vector<std::thread> controllerThreadPool;

      public:
        ThreadOrchestrator() {}
        const std::string &getTag() { return TAG; }
        std::string getDesc() { return "temp"; }
        std::string getStatus() { return "rew"; }
        int getStatusInt() { return 0; }
        bool getAutoRestartFlag() { return false; }

        int pushControllerPool(std::shared_ptr<Controller> controller)
        {
            std::shared_ptr<ThreadContainer> tc(new ThreadContainer(controller));
            threadContainerPool.push_back(tc);
            return 0;
        }

        int startThread()
        {
            CpuLimiter limiter(5);

            for (unsigned int i = 0; i < threadContainerPool.size(); i++) {
                INFO(getTag(), getDesc(), stringbuilder() << threadContainerPool[i]->getTag() << " thread started with code: { " << threadContainerPool[i]->startThread() << " }");
            }
            INFO(getTag(), getDesc(), stringbuilder() << "Total Threads Started: { " << threadContainerPool.size() << " }");
            while (1) {
                for (unsigned int i = 0; i < threadContainerPool.size(); i++) {
                    const std::string indexTag = threadContainerPool[i]->getTag();
                    int statusCode = threadContainerPool[i]->getStatusCode();
                    bool autoRestart = threadContainerPool[i]->getAutoRestartFlag();

                    if (statusCode > 0) {
                        DEBUG(getTag(), getDesc(), stringbuilder() << indexTag << " Exit Status Code: { " << statusCode << " }");
                        if (autoRestart == true) {
                            DEBUG(getTag(), getDesc(), stringbuilder() << "Restarting Thread: { " << indexTag << " Auto-Restart-Flag: { " << autoRestart << " }");
                            int ret = threadContainerPool[i]->startThread();
                            if (ret != 0) {
                                ERROR(getTag(), getDesc(), stringbuilder() << indexTag << " Restart Return Code: { " << ret << " }");
                            }
                        }
                        else {
                            WARNING(getTag(), getDesc(), stringbuilder() << indexTag << " Perhaps Turn On Auto-Restart-Flag: { " << autoRestart << " }");
                            WARNING(getTag(), getDesc(), stringbuilder() << indexTag << " Setting Dead Thread Status Code");
                            threadContainerPool[i]->setDeadThreadStatusCode();
                        }
                    }
                    limiter.CalculateAndSleep();
                }
            }
            return 0;
        }
    };
} // namespace Engine
#endif