#pragma once

#include "Vector3.h"

#include <string>

class Matrix4;

class Vector4 : public Vector3
{
protected:
    double w;

public:
	Vector4();
    Vector4(float *v, double w);
    Vector4(double x, double y, double z);
	Vector4(double x, double y, double z, double w);
    Vector4(const Vector3&);
	~Vector4();

    Vector4& operator=(const Vector4 &);
    double& operator[](int index);
    double operator[](int index) const;
    Vector4 mut_r(const Matrix4&); /* Multiply as a row vector */
    double operator*(const Vector4& v) const;
    inline Vector3 operator*(double s) const { return Vector3::operator*(s); }
    void dehomogenize();
    void set(double x, double y, double z, double w);
    
	std::string toString() const;
};

