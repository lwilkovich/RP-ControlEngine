#ifndef __THREADCONTAINER_H__
#define __THREADCONTAINER_H__

#include "../Controllers/Controller.h"
#include "_Logger.h"
#include "_String.h"
#include <iostream>
#include <memory>
#include <pthread.h>
#include <thread>

#define _THREAD_PREFIX "ce*"

namespace Engine {
    class ThreadContainer {
      private:
        const std::string TAG = "ThreadContainer";
        std::shared_ptr<Controller> controller;
        pid_t pid;
        int statusCode = 0;
        std::thread controllerThread;

        void threadRuntime() {
            std::string threadName = _THREAD_PREFIX + controller->getTag().substr(0, 11);
            pthread_setname_np(pthread_self(), threadName.c_str());
            pid = getpid();
            INFO(getTag(), "", stringbuilder() << "Thread Name Set: { " << threadName << " } PID: { " << pid << " }");
            try {
                statusCode = controller->startThread();
            } catch (const std::exception &ex) {
                std::cout << "Error occurred: " << ex.what() << std::endl;
                statusCode = 1;
            }
        }

      public:
        ThreadContainer(std::shared_ptr<Controller> shared_c) { controller = shared_c; }
        ~ThreadContainer() {}
        int startThread() {
            try {
                if (controllerThread.joinable()) {
                    controllerThread.join();
                }
                statusCode = 0;
                controllerThread = std::thread(&ThreadContainer::threadRuntime, this);
            } catch (const std::exception &ex) {
                std::cout << "Error occurred: " << ex.what() << std::endl;
                statusCode = 1;
            }
            return statusCode;
        }
        const std::string getTag() { return stringbuilder() << TAG << '.' << controller->getTag(); }
        int getStatusCode() { return statusCode; }
        bool getAutoRestartFlag() { return controller->getAutoRestartFlag(); }
        void setDeadThreadStatusCode() { statusCode = -1; }
    };
} // namespace Engine
#endif