#ifndef __LOGQUEUE_H__
#define __LOGQUEUE_H__

#include <queue>
#include <mutex>

namespace _log {
    class LogQueue {
        private:
            std::string mClass = "LogQueue";
            std::queue<std::array<std::string, 4>> mQueue;
            std::mutex popLock;
            std::mutex pushLock;
        public:
            LogQueue() {}
            LogQueue(const LogQueue &other) {
                mQueue = other.mQueue;
            }
            LogQueue &operator=(const LogQueue& other) {
                if (this != &other) {
                    mQueue = other.mQueue;
                }
                return *this;
            }

            std::string getClass() {
                return mClass;
            }
            /* Static access method. */
      
            static LogQueue& getInstance() {
                static LogQueue instance;
                return instance;
            }

            std::array<std::string, 4> front() {
                popLock.lock();
                return mQueue.front();
                popLock.unlock();
            }

            std::array<std::string, 4> pop() {
                popLock.lock();
                std::array<std::string, 4> popped = mQueue.front();
                mQueue.pop();
                popLock.unlock();
                return popped;
            }

            bool empty() {
                popLock.lock();
                bool ret = mQueue.empty();
                popLock.unlock();
                return ret;
            }

            void push(std::array<std::string, 4> value) {
                pushLock.lock();
                mQueue.push(value);
                pushLock.unlock();
            }

            void push(std::string s1, std::string s2 = "", std::string s3 = "", std::string s4 = "") {
                pushLock.lock();
                mQueue.push({s1, s2, s3, s4});
                pushLock.unlock();
            }
    };
}
#endif