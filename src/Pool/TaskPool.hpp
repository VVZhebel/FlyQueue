#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>
#include "Semaphore.hpp"
#include <iostream>

template <class Task>
class TaskPool{
  private:
    TaskPool():Active(true){}
    ~TaskPool(){}
    TaskPool(TaskPool const &)=delete;
    TaskPool& operator= (TaskPool const &)=delete;

    std::queue<Task*>           Tasks;
    std::mutex                  vectorLock;
    std::condition_variable     queue_check;
    Semaphore                   semaphore;
    bool Active;
  public:
    static TaskPool& Pool(){
        static TaskPool Pool;
        return Pool;
    }
    void pushTask(Task* T){
        vectorLock.lock();
        Tasks.push(T);
        semaphore.add();
        vectorLock.unlock();
    }
    Task* popTask(){
        Task* result;
        semaphore.enter();
        if (!Active) throw -1;
        vectorLock.lock();
        result = Tasks.front();
        Tasks.pop();
        vectorLock.unlock();
        return result;
    }
    void StopPool(){
        std::cout << "Stop task pool" << std::endl;
        Active = false;
        semaphore.unleash();
    }
};