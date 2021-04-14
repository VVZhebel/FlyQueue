#pragma once

#include <iostream>
#include <vector>
#include "../Pool/TaskPool.hpp"
#include "../Pool/ProgressPool.hpp"
#include "../Task/Task.hpp"
#include "../Task/Counter.hpp"
#include "../Task/TaskWithTimer.hpp"
#include "../MaskStorage.hpp"
#include "../ResultLog/LogPool.hpp"
#include "../ResultLog/LogTask.hpp"

#define LONG_SIZE 8

#define byte unsigned char

class Handler{
    private:
        void getTask();
        void pushTask(long, std::vector<byte>&);
        void ping(long);
        void pushResult(long, std::vector<byte>&);
    public:
        Handler();
        ~Handler();
        std::vector<byte> handle(std::vector<byte>&);
        std::vector<byte> Response;
};