#include "Sphere.h"

#include "GLee.h"

Sphere::Sphere() : Sphere(1.0) {}

Sphere::Sphere(double radius)
{
    this->radius = radius;
    this->slices = 64;
    this->stacks = 64;
    this->set_bouding_box(Vector3(radius, 0, 0), Vector3(0, radius, 0), Vector3(0, 0, radius));
}

Sphere::~Sphere()
{
}

void Sphere::render()
{
    glutSolidSphere(this->radius, this->slices, this->stacks);
}
