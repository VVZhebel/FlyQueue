#pragma once
#include <vector>
#include <cstring>
#include "LongBufUnion/Union.hpp"

struct MaskStorage{
    LongBuf parentId;
    std::vector<unsigned char> Mask;

    std::vector<unsigned char> toMsg(){
        std::vector<unsigned char> Res;
        Res.insert(Res.end(), parentId.Buf, parentId.Buf + 8);
        
        Res.insert(Res.end(), Mask.begin(), Mask.end());

        return Res;
    }
};