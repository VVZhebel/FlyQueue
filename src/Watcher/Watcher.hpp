#pragma once
#include <iostream>
#include <thread>
#include <signal.h>
#include <unistd.h>

#include "../Pool/ProgressPool.hpp"
#include "../Task/Task.hpp"
#include "../Task/TaskWithTimer.hpp"
#include "../MaskStorage.hpp"

#define SLEEP_TIME 20

class Watcher{
    private:
        Watcher();
        ~Watcher();
        Watcher(Watcher const&) = delete;
        Watcher& operator= (Watcher const&) = delete;

        std::thread T;
        bool Active;
    public:
        static Watcher& W();
        void start();
        void stop();
        bool is_active();
};