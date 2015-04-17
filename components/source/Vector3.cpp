#include "Vector3.h"
#include "Utility.h"

#include <sstream>
#include <iostream>

#define X v[0]
#define Y v[1]
#define Z v[2]

Vector3::Vector3() : Vector3(0, 0, 0) {}

Vector3::Vector3(float *v) : Vector3(v[0], v[1], v[2]) {}

Vector3::Vector3(double x, double y, double z)
{
    X = x;
    Y = y;
    Z = z;
}

Vector3::~Vector3()
{
}

Vector3& Vector3::operator=(const Vector3& v)
{
    this->X = v[0];
    this->Y = v[1];
    this->Z = v[2];
    return (*this);
}

Vector3 Vector3::operator+(const Vector3& vector) const
{
	return Vector3(X + vector[0], Y + vector[1], Z + vector[2]);
}

Vector3& Vector3::operator+=(const Vector3& vector)
{
    X += vector[0];
    Y += vector[1];
    Z += vector[2];
    return (*this);
}

Vector3 Vector3::operator-(const Vector3& vector) const
{
    return Vector3(X - vector[0], Y - vector[1], Z - vector[2]);
}

Vector3& Vector3::operator-=(const Vector3& vector)
{
    X -= vector[0];
    Y -= vector[1];
    Z -= vector[2];
    return (*this);
}

Vector3 Vector3::operator-() const
{
    return Vector3(-X, -Y, -Z);
}

Vector3 Vector3::operator*(double s) const
{
    Vector3 result = (*this);
    result.scale(s);
    return result;
}

Vector3 Vector3::operator/(double s) const
{
    return (*this) * (1 / s);
}

double Vector3::operator[](int index) const
{
    return v[index];
}

double& Vector3::operator[](int index)
{
    return v[index];
}

void Vector3::negate(){
    X = -X;
	Y = -Y;
	Z = -Z;
}

void Vector3::scale(double s){
    X = s * X;
	Y = s * Y;
	Z = s * Z;
}

double Vector3::dot(const Vector3& a, const Vector3& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double Vector3::dot(const Vector3& b) const
{
    return X * b[0] + Y * b[1] + Z * b[2];
}

float Vector3::angle(const Vector3& b) const
{
    return rad2degree(angle_rad(b));
}

float Vector3::angle_rad(const Vector3& b) const
{
    float tmp = this->dot(b) / (this->length() * b.length());
    tmp = tmp > 1.0 ? 1.0 : tmp;
    tmp = tmp < -1.0 ? -1.0 : tmp;
    return acos(tmp);
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
	return Vector3(a[1] * b[2] - a[2] * b[1],
		a[2] * b[0] - a[0] * b[2],
		a[0] * b[1] - a[1] * b[0]);
}

Vector3 Vector3::cross(const Vector3& b) const
{
    return Vector3(Y * b[2] - Z * b[1],
        Z * b[0] - X * b[2],
        X * b[1] - Y * b[0]);
}

double Vector3::length() const{
	return sqrt(X * X + Y * Y + Z * Z);
}

double Vector3::length_sq() const
{
    return X * X + Y * Y + Z * Z;
}

Vector3& Vector3::normalize()
{
	scale(1 / this->length());
    return (*this);
}

void Vector3::set(double x, double y, double z)
{
    X = x;
    Y = y;
    Z = z;
}

double *Vector3::get_pointer()
{
    return v;
}

void Vector3::get_pointerf(float *f)
{
    f[0] = v[0];
    f[1] = v[1];
    f[2] = v[2];
}

void Vector3::print(const std::string& s) const{
	std::cout << s << " : " << this->toString() << std::endl;
}

std::string Vector3::toString() const{
	std::stringstream buffer;
	buffer << "[" << X << "," << Y << "," << Z << "]";
	return buffer.str();
}
