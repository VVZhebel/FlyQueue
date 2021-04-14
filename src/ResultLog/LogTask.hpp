#pragma once
#include "../LongBufUnion/Union.hpp"

struct LogTask{
    LongBuf Id;
    LongBuf parentId;
    std::vector<unsigned char> Result;

    std::vector<unsigned char> toMsg(){
        std::vector<unsigned char> Result;

        Result.insert(Result.end(), Id.Buf, Id.Buf + 8);
        Result.insert(Result.end(), parentId.Buf, parentId.Buf + 8);
        Result.insert(Result.end(), this->Result.begin(), this->Result.end());

        return Result;
    }
};