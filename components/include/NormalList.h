#pragma once

#include "GeometryList.h"

class NormalList :
    public GeometryList<float>
{
public:
    NormalList();
    ~NormalList();
};

