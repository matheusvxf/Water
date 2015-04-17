#pragma once

#include "DataType.h"
#include "Geode.h"

class Wall :
    public Geode
{
protected:
    double height;
    double width;
    UINT32 slices;
    double *vertices;

    GLuint texture;
public:
    Wall();
    Wall(double height, double width);
    Wall(double height, double width, UINT32 slices);
    Wall(double height, double width, UINT32 slice, UINT32 texture_id);
    virtual ~Wall();

    void render();
};

