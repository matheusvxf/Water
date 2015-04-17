#pragma once

#include "Vector3.h"

#include <list>

class IUpdatable
{
public:
    static std::list<IUpdatable*> list;

    IUpdatable();
    ~IUpdatable();

    virtual void update() = 0;
    virtual void apply_force(const Vector3& force) = 0;
    static void call_update();
};

