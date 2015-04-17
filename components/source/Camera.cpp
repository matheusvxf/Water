#include "Camera.h"

#include <iostream>

Camera::Camera()
{
    reset();
}

Camera::Camera(const Vector4& e, const Vector4& d, const Vector4& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    calculate_camera_matrix();
}

Camera::~Camera()
{
}

void Camera::calculate_camera_matrix()
{
    Vector3 x, y, z;
    Matrix4 R, T;
    z = e - d; z.normalize();
    x = up.cross(z); x.normalize();
    y = z.cross(x); y.normalize();

    R.identity();
    R.setColumn(0, x);
    R.setColumn(1, y);
    R.setColumn(2, z);

    T.identity();
    T.setColumn(3, e);
    T.set(3, 3, 1);
    camera2world = R * T;

    T.setColumn(3, -e);
    T.set(3, 3, 1);
    world2camera = R.transpose() * T * model2world;
}

Matrix4& Camera::getMatrix()
{
    return world2camera;
}

const double* Camera::getGLMatrix()
{
    return getMatrix().getPointer();
}

void Camera::set_center_of_proj(const Vector4& e)
{
    this->e = e;
    calculate_camera_matrix();
}

Vector4& Camera::center_of_proj()
{
    return e;
}

Vector4& Camera::center_of_proj(const Vector4& e)
{
    set_center_of_proj(e);
    return this->e;
}

void Camera::set_center_of_proj(double x, double y, double z)
{
    this->e = Vector4(x, y, z);
    calculate_camera_matrix();
}

Vector4& Camera::look_at()
{
    return d;
}

void Camera::look_at(const Vector4& d)
{
    this->d = d;
    calculate_camera_matrix();
}

void Camera::look_at(double x, double y, double z)
{
    this->d = Vector4(x, y, z);
    calculate_camera_matrix();
}

Vector3& Camera::get_up()
{
    return up;
}

void Camera::set_up(const Vector4& up)
{
    this->up = up;
    calculate_camera_matrix();
}

void Camera::set_up(double x, double y, double z)
{
    this->up = Vector4(x, y, z);
    calculate_camera_matrix();
}

void Camera::reset()
{
    e = Vector4(-5, 10, 20);
    d = Vector4(0, 0, -1);
    up = Vector4(0, 1, 0);
    calculate_camera_matrix();
}