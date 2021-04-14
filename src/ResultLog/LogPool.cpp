#include "LogPool.hpp"

LogPool::LogPool() : Active(true)
{
    haveTasks.lock();
}

LogPool::~LogPool()
{
    while (Tasks.size() > 0)
    {
        LogTask *T = Tasks.front();
        Tasks.pop();
        delete T;
    }
}

LogPool& LogPool::Pool()
{
    static LogPool pool;
    return pool;
}

void LogPool::pushTask(LogTask *T)
{
    std::unique_lock<std::mutex> Guard(vectorLock);
    Tasks.push(T);
    haveTasks.unlock();
}

LogTask *LogPool::popTask()
{
    haveTasks.lock();
    if (!Active)
        return nullptr;
    std::unique_lock<std::mutex> Guard(vectorLock);
    LogTask *temp = Tasks.front();
    Tasks.pop();
    if (Tasks.size() > 0)
        haveTasks.unlock();
    return temp;
}

void LogPool::StopService()
{
    this->Active = false;
    haveTasks.unlock();
}