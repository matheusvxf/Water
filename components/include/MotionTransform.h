#pragma once

#include "Object.h"
#include "MatrixTransform.h"
#include "IUpdatable.h"

class MotionTransform :
    public MatrixTransform,
    public Object
{
public:
    MotionTransform();
    virtual ~MotionTransform();

    void update();
    void apply_force(const Vector3& v);
};

