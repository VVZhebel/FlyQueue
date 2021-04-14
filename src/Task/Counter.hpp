#pragma once
#include <atomic>

class Counter{
    private:
        std::atomic<long> counter;
    public:
        Counter():counter(0){}
        ~Counter(){}
        long getCounter(){
            return counter++;
        }
        void setCounter(long value){
            counter = value;
        }
};