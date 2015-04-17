#pragma once
#include "Group.h"
class MatrixTransform :
    public Group
{
protected:
    Matrix4* M;

public:
    MatrixTransform();
    MatrixTransform(Matrix4 *M);
   virtual ~MatrixTransform();

    void setTransform(Matrix4* M);
    void draw(const Matrix4& C);
    virtual Matrix4 get_model_view() const;
};

