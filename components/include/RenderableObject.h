#pragma once

#include "Object.h"
#include "Group.h"

class RenderableObject :
    public Object,
    public Group
{
public:
    RenderableObject();
    virtual ~RenderableObject();

    virtual void update() = 0;
};

