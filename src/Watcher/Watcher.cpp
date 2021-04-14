#include "Watcher.hpp"

void signal_alrm(int signum){
    std::cout << "Stop watcher" << std::endl;
}

Watcher::Watcher(){
    signal(SIGALRM,signal_alrm);
    Active = true;
}
Watcher::~Watcher(){
    T.join();
}

void Watcher::start(){
    T = std::thread([](){
        while (Watcher::W().is_active()){
            sleep(SLEEP_TIME);
            std::vector<TaskWithTimer<Task<MaskStorage>>*> Old = ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().popOld();
            for (auto &i:Old){
                TaskPool<Task<MaskStorage>>::Pool().pushTask(i->T);
                delete i;
            }
        }
    });
}

Watcher& Watcher::W(){
    static Watcher W;
    return W;
}


void Watcher::stop(){
    Active = false;
    pthread_kill(T.native_handle(), SIGALRM);
}

bool Watcher::is_active(){
    return Active;
}
