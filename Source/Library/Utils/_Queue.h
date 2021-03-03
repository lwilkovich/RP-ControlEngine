#ifndef ___QUEUE_H__
#define ___QUEUE_H__

#include <mutex>
#include <queue>

namespace _Queue {
    template <class T> class Queue {
      private:
        std::queue<T> safeQueue;
        std::mutex frontLock;
        std::mutex backLock;

      public:
        Queue() {}
        Queue(const Queue &other) { safeQueue = other.safeQueue; }
        Queue &operator=(const Queue &other) {
            if (this != &other) {
                safeQueue = other.safeQueue;
            }
            return *this;
        }

        T pop() {
            frontLock.lock();
            T x = safeQueue.front();
            safeQueue.pop();
            frontLock.unlock();
            return x;
        }
        void push(T x) {
            frontLock.lock();
            safeQueue.push(x);
            frontLock.unlock();
            return;
        }
        int size() { return safeQueue.size(); }
        T front() {
            frontLock.lock();
            T x = safeQueue.front();
            frontLock.unlock();
            return x;
        }
    };
} // namespace _Queue
#endif