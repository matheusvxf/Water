#pragma once

#include "Geode.h"
#include "Shader.h"

class Box :
    public Geode
{
private:
    void init(float *points);
protected:
    float *points;
    GLuint vbo;
    GLuint vao;
    Shader *shader;
public:
    Box();
    Box(float *points);
    virtual ~Box();

    virtual void render();
};