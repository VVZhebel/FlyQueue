#include "Handler.hpp"

Handler::Handler(){}
Handler::~Handler(){
}
std::vector<byte> Handler::handle(std::vector<byte>& R){
    LongBuf Id;
    std::vector<byte> Data;
    switch (R[0])
    {
    case 1:
        this->getTask();
        break;
    case 2:
        std::memcpy(&Id, R.data()+1, LONG_SIZE);
        Data.insert(Data.end(), R.begin()+LONG_SIZE+1, R.end());
        this->pushTask(Id.Id,Data);
        break;
    case 3:
        std::memcpy(&Id, R.data()+1, LONG_SIZE);
        this->ping(Id.Id);
        break;
    case 4:
        std::memcpy(&Id, R.data()+1, LONG_SIZE);
        Data.insert(Data.end(), R.begin()+LONG_SIZE+1, R.end());
        this->pushResult(Id.Id, Data);
        break;
    case 5:
        LongBuf TaskSize;
        LongBuf PoolSize;
        TaskSize.Id = ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().getSize();
        PoolSize.Id = TaskPool<Task<MaskStorage>>::Pool().getSize();
        Response.insert(Response.end(), TaskSize.Buf, TaskSize.Buf + 8);
        Response.insert(Response.end(), PoolSize.Buf, PoolSize.Buf + 8);
        break;
    default:
        break;
    }

    return Response;
}

void Handler::getTask(){
    Task<MaskStorage>* Data = TaskPool<Task<MaskStorage>>::Pool().popTask();
    TaskWithTimer<Task<MaskStorage>>* Progress = new TaskWithTimer<Task<MaskStorage>>(Data);
    ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().pushTask(Data->Id.Id, Progress);

    Response = Data->toMsg();
}

void Handler::pushTask(long Id, std::vector<byte>& Mask){
    MaskStorage M;
    M.Mask = Mask;
    M.parentId.Id = Id;

    Task<MaskStorage>* Data = new Task<MaskStorage>(M);
    TaskPool<Task<MaskStorage>>::Pool().pushTask(Data);
}

void Handler::ping(long Id){
    auto TaskToPing = ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().findTask(Id);
    if (TaskToPing != 0)
        TaskToPing->updateTimer();
}

void Handler::pushResult(long Id, std::vector<byte>& Result){
    TaskWithTimer<Task<MaskStorage>>* TaskToLog = ProgressPool<TaskWithTimer<Task<MaskStorage>>>::Pool().popTask(Id);
    if (!TaskToLog) return;
    LogTask* Log = new LogTask();
    Log->Id = TaskToLog->T->Id;
    Log->parentId = TaskToLog->T->D.parentId;
    Log->Result = Result;

    LogPool::Pool().pushTask(Log);
    delete TaskToLog->T;
    delete TaskToLog;
}