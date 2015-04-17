#pragma once

#include "Geode.h"
#include "Shader.h"

class Patch :
    public Geode
{
private:
    void generate_patch();

protected:

    float *vertice;
    float *normal;
    float *normal_line;
    UINT32 n_vertice;
    Vector4 points[16];
    float phase;
    Shader *reflection;
public:
    Patch();
    Patch(float phase);
    ~Patch();

    void update();
    void render();
};

