#include <iostream>
#include "Server/Server.hpp"
#include "Watcher/Watcher.hpp"
#include "Handler/Handler.hpp"
#include "ResultLog/Logger.hpp"
#include "Pool/TaskPool.hpp"

int main(){
    Task<MaskStorage>::initCounter();

    for (unsigned char i=0; i<10; i++){
        MaskStorage M;
        M.parentId.Id = -1;
        M.Mask = std::vector<unsigned char>({i,i});

        Task<MaskStorage>* T = new Task<MaskStorage>(M);
        TaskPool<Task<MaskStorage>>::Pool().pushTask(T);
    }

    Watcher::W().start();
    Logger L;
    L.Start();

    Server<Handler> S(5555,128);
    S.Start();

    return 0;
}