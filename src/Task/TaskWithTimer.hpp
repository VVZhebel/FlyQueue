#pragma once
#include <ctime>

template<class Task>
class TaskWithTimer{
    public:
        Task* T;
        std::time_t lasttime;
        TaskWithTimer(Task* T){
            this->T = T;
            lasttime = time(0);
        }

        void releaseTask(){
            delete T;
        }
        void updateTimer(){
            lasttime = time(0);
        }
};