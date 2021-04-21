#include "Watcher.hpp"

void signal_alrm(int signum){
    std::cout << "Stop watcher" << std::endl;
}

Watcher::Watcher(){
    signal(SIGALRM,signal_alrm);
    Active = true;
}
Watcher::~Watcher(){
    std::cout << "Watcher destructor" << std::endl;
    if (T1.joinable())
        T1.join();
    if (T2.joinable())
        T2.join();
    std::cout << "Watcher destructor finished" << std::endl;
}

void Watcher::start(){
    T1 = std::thread([](){
        while (Watcher::W().is_active()){
            sleep(SLEEP_TIME);
            std::vector<TaskWithTimer<Task<MaskStorage>>*> Old = ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().popOld();
            for (auto &i:Old){
                TaskPool<Task<MaskStorage>>::Pool().pushTask(i->T);
                delete i;
            }
        }
        std::cout << "End of clean task" << std::endl;
    });
    T2 = std::thread([](Watcher* W){
        while (Watcher::W().is_active()){
            sleep(SLEEP_TIME);
            std::cout << "TaskPool: " << TaskPool<Task<MaskStorage>>::Pool().getSize() << std::endl;
            std::cout << "ProgressPool: " << ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().getSize() << std::endl;
            if ((TaskPool<Task<MaskStorage>>::Pool().getSize() == 0) && (ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().getSize() == 0)){
                std::cout << "Stop server" << std::endl;
                W->stop_all_pool();
                std::cout << "Pool stoped" << std::endl;
                W->stop();
                std::cout << "Watcher stoped" << std::endl;
                return;
            }
        }
        std::cout << "End of stop task" << std::endl;
    }, this);
}

void Watcher::start(Logger* L, Server<Handler>* S){
    this->L = L;
    this->S = S;

    this->start();
}

Watcher& Watcher::W(){
    static Watcher W;
    return W;
}


void Watcher::stop(){
    Active = false;
    if (T1.joinable())
        pthread_kill(T1.native_handle(), SIGALRM);
    if (T2.joinable())
        pthread_kill(T2.native_handle(), SIGALRM);
}

bool Watcher::is_active(){
    return Active;
}

void Watcher::stop_all_pool(){
    this->S->Stop();
    this->L->Stop();
}

void Watcher::wait(){
    if (T1.joinable())
        T1.join();
    if (T2.joinable())
        T2.join();
}
