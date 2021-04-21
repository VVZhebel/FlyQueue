#include "LogPool.hpp"

LogPool::LogPool() : Active(true)
{
    haveTasks.lock();
}

LogPool::~LogPool()
{
    std::cout << this->Tasks.size() << std::endl;
    while (this->Tasks.size()>0)
    {
        LogTask *T = this->Tasks.front();
        this->Tasks.pop();
        if (T)
            delete T;
    }
    std::cout << "Log pool stopped" << std::endl;
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
    if (!Active){
        return nullptr;
    }
    haveTasks.lock();
    if (!Active){
        return nullptr;
    }
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