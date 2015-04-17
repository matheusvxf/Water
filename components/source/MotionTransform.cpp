#include "MotionTransform.h"


MotionTransform::MotionTransform()
{
    this->M = &this->model2world;
}

MotionTransform::~MotionTransform()
{
}

void MotionTransform::update()
{
    Object::update();

}

void MotionTransform::apply_force(const Vector3& v)
{
    Object::apply_force(v);
}