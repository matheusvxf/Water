#include "Object.h"
#include "DataType.h"
#include "Config.h"

#include <iostream>
#include <string>
#include <sstream>

Object::Object() : position(0, 0, 0, 1)
{
    reset();
    model2world.identity();
}

Object::~Object()
{
}

void Object::update()
{
    force -= velocity * friction;
    acceleration += force / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    force.set(0, 0, 0, 0);
    acceleration.set(0, 0, 0, 0);
    
    updateMatrix();
}

void Object::render()
{
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixd(this->model2world.getPointer());
}

Matrix4& Object::getMatrix()
{
    return model2world;
}

Matrix4& Object::get_model2world()
{
    return model2world;
}

void Object::rotateX(double deg)
{
    angleX += deg;
    if (angleX > 360.0 || angleX < -360.0)
        angleX = 0.0;
}

void Object::rotateY(double deg)   // deg is in degrees
{
    angleY += deg;
    if (angleY > 360.0 || angleY < -360.0)
        angleY = 0.0;
}

void Object::rotateZ(double deg)
{
    angleZ += deg;
    if (angleZ > 360.0 || angleZ < -360.0)
        angleZ = 0.0;
}

void Object::rotate(float angle, float x, float y, float z)
{
    return rotate(angle, Vector3(x, y, z));
}

void Object::rotate(float angle, const Vector3& a)
{
    static Matrix4 rot;

    rot.makeRotate(angle, a);
    rotate_axis = rot * rotate_axis;
    updateMatrix();
}

void Object::clean_rotation()
{
    rotate_axis.identity();
    updateMatrix();
}

void Object::addScale(double sca)
{
    scale += sca;
    if (scale <= 0.000000001)
        scale = 0.000000001;
}

void Object::updateMatrix()
{
    Matrix4 m_scale, m_translate, m_rotateX, m_rotateY, m_rotateZ;

    m_translate.makeTranslate(position[0], position[1], position[2]);
    m_scale.makeScale(scale, scale, scale);
    m_rotateX.makeRotateX(angleX);
    m_rotateY.makeRotateY(angleY);
    m_rotateZ.makeRotateZ(angleZ);
    
    model2world = rotate_axis * m_rotateX * m_rotateY * m_rotateZ * m_translate * m_scale;
}

void Object::apply_force(const Vector3& force)
{
    this->force += model2world * force;
}

void Object::apply_acceleration(const Vector3& acceleration)
{
    this->acceleration += model2world * acceleration;
}

Vector4& Object::getPosition()
{
    return this->position;
}

double Object::getSpeed() const
{
    return this->velocity.length();
}

void Object::reset()
{
    position.set(0, 0, 0, 1);
    force = velocity = position;
    angleX = 0.0;
    angleY = 0.0;
    angleZ = 0.0;
    scale = 1.0;
    mass = 0.5;
    friction = 4;
    rotate_axis.identity();
}

void Object::print()
{
    std::cout << toString() << std::endl;
}

std::string Object::toString()
{
    std::stringstream buffer;
    buffer << "position: " << this->position.toString() << std::endl;
#if (DEBUG == TRUE)
    buffer << "matrix: " << std::endl << this->model2world.toString();
#endif
    return buffer.str();
}