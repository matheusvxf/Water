#pragma once

#include "Geode.h"
#include "DataType.h"

class Cone :
    public Geode
{
protected:
    float base, height;
    UINT32 slices, stacks;
public:
    Cone();
    Cone(float base, float height);
    Cone(float base, float height, UINT32 slices, UINT32 stacks);
    virtual ~Cone();

    virtual void render();
};

