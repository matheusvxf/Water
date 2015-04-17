#include "Cube.h"
#include "Vector3.h"

#include <GL/glut.h>

Cube::Cube() : Cube(1) {}

Cube::Cube(double size)
{
    double radius = sqrt(size * size / 4.0 + size * size / 4.0);
    this->size = size;
    this->set_bouding_box(Vector3(radius, 0, 0), Vector3(0, radius, 0), Vector3(0, 0, radius));
}

Cube::~Cube() {}

void Cube::update()
{
}

void Cube::render()
{
    glutSolidCube(size);
}
