#pragma once
#include <thread>
#include "LogPool.hpp"
#include "../Task/Task.hpp"
#include "../MaskStorage.hpp"
#include <fstream>

#define RESULT_FILE "result.bin"

class Logger
    {
    private:
        std::thread Worker;
    public:
        Logger();
        ~Logger();
        void Start();
        void Stop();
    };