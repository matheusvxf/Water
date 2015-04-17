#include "MatrixTransform.h"

MatrixTransform::MatrixTransform() : MatrixTransform(nullptr) {}

MatrixTransform::MatrixTransform(Matrix4 *M)
{
    this->M = M;
}

MatrixTransform::~MatrixTransform()
{
    delete M;
}

void MatrixTransform::setTransform(Matrix4* M)
{
    this->M = M;
}

void MatrixTransform::draw(const Matrix4& C)
{
    Group::draw(C * *M);
}

Matrix4 MatrixTransform::get_model_view() const
{
    if (owner == nullptr)
        return *M;
    else
        return owner->get_model_view() * *M;
}
