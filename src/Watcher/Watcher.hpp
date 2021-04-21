#pragma once
#include <iostream>
#include <thread>
#include <signal.h>
#include <unistd.h>

#include "../Pool/ProgressPool.hpp"
#include "../Task/Task.hpp"
#include "../Task/TaskWithTimer.hpp"
#include "../MaskStorage.hpp"

#include "../ResultLog/Logger.hpp"
#include "../Server/Server.hpp"
#include "../Handler/Handler.hpp"

#define SLEEP_TIME 10

class Watcher{
    private:
        Watcher();
        ~Watcher();
        Watcher(Watcher const&) = delete;
        Watcher& operator= (Watcher const&) = delete;

        std::thread T1;
        std::thread T2;
        bool Active;

        Server<Handler>* S;
        Logger* L;

        
    public:
        static Watcher& W();
        void start();
        void start(Logger*, Server<Handler>*);
        void stop();
        bool is_active();      

        void stop_all_pool();
        void wait();
};