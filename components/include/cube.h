#pragma once

#include "Geode.h"

class Cube :
    public Geode
{
protected:
    double size;

public:
    Cube();
    Cube(double size);
    virtual ~Cube();

    void update();
	void render();
};
