#pragma once
#include <mutex>
#include <queue>
#include <iostream>
#include <unordered_map>

#include "TaskPool.hpp"

#define EXPIRE 10

template <class Task>
class ProgressPool{
  private:
    ProgressPool(){}
    ~ProgressPool(){
        std::cout << "ProgressPool destructor" << std::endl;
    }
    ProgressPool(ProgressPool const &)=delete;
    ProgressPool& operator= (ProgressPool const &)=delete;

    std::unordered_map<long,Task*>  Index;
    std::mutex                      Lock;
  public:
    static ProgressPool& Pool(){
        static ProgressPool Pool;
        return Pool;
    }
    void pushTask(long Id, Task* T){
        std::unique_lock<std::mutex> Guard(Lock);
        Index.insert({Id, T});
    }
    Task* popTask(long Id){
        if (Index.find(Id) == Index.end()) return 0;
        std::unique_lock<std::mutex> Guard(Lock);
        Task* T = Index[Id];
        Index.erase(Id);
        return T;
    }
    Task* findTask(long Id){
        if (Index.find(Id) == Index.end()) return 0;
        return Index[Id];
    }
    std::vector<Task*> popOld(){
        std::vector<long> toDelete;
        std::vector<Task*> Old;
        std::unique_lock<std::mutex> Guard(Lock);
        std::time_t Now = time(0);
        for (auto i=Index.begin(); i!=Index.end(); i++)
            if ((Now - i->second->lasttime)>EXPIRE){
                Old.push_back(i->second);
                toDelete.push_back(i->first);
            }
        for (auto &i:toDelete)
            Index.erase(i);
        return Old;
    }
    int getSize(){
        return Index.size();
    }
};