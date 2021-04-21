#pragma once
#include <mutex>
#include <queue>
#include <iostream>
#include "LogTask.hpp"

class LogPool
{
private:
    LogPool();
    ~LogPool();
    LogPool(LogPool const &) = delete;
    LogPool &operator=(LogPool const &) = delete;

    std::queue<LogTask *> Tasks = {};
    std::mutex vectorLock;
    std::mutex haveTasks;

public:
    static LogPool& Pool();
    void pushTask(LogTask *T);
    LogTask* popTask();
    void StopService();
    bool Active;
};