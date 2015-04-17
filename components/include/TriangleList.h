#pragma once

#include "GeometryList.h"

class TriangleList :
    public GeometryList<UINT32>
{
public:
    TriangleList();
    virtual ~TriangleList();
};

