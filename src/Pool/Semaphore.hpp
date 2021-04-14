#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore{
    public:
        explicit Semaphore() : count(0) {}

        void enter(){
            std::unique_lock<std::mutex> lock(locker);
            while (count == 0) cv.wait(lock);
            --count;
        }

        void add(){
            std::lock_guard<std::mutex> lock(locker);
            ++count;
            cv.notify_all();
        }

        inline void unleash(){
            count = -1;
            cv.notify_all();
        }

    private:
        int                         count;
        std::mutex                  locker;
        std::condition_variable     cv;
};