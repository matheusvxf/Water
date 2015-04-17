#pragma once
#include "Geode.h"

class Sphere :
    public Geode
{
protected:
    double radius;
    double slices;
    double stacks;
public:
    Sphere();
    Sphere(double radius);
    virtual ~Sphere();

    virtual void render();
};

