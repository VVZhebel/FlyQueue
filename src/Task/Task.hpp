#pragma once

#include "Counter.hpp"
#include <cstring>
#include "../LongBufUnion/Union.hpp"

template <class Data>
class Task
{
private:
  static Counter _counter;
public:
  LongBuf Id;
  Data D;
  Task(Data &D)
  {
    Id.Id = Task<Data>::_counter.getCounter();
    this->D = D;
  }
  ~Task()
  {
  }
  std::vector<unsigned char> toMsg()
  {
    std::vector<unsigned char> Res;

    std::vector<unsigned char> M = D.toMsg();

    Res.insert(Res.end(), Id.Buf, Id.Buf + 8);
    Res.insert(Res.end(), M.begin(), M.end());
    return Res;
  }

  static void initCounter(){
    Task<Data>::_counter.setCounter(0);
  }
};

template <class Data> Counter Task<Data>::_counter;