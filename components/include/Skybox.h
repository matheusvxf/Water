#pragma once
#include "Group.h"
#include "Wall.h"

class Skybox :
    public Group
{
protected:
    GLuint texture[7];
public:
    Skybox();
    ~Skybox();

    virtual void draw(const Matrix4& C);
};

