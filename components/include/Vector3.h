#pragma once

#include <string>

class Vector3
{
protected:
    double v[3];
public:
    Vector3();
    Vector3(float *);
	Vector3(double x, double y, double z);
	~Vector3();
	
	Vector3 operator+(const Vector3&) const;
    Vector3& operator+=(const Vector3& vector);
	Vector3 operator-(const Vector3&) const;
    Vector3& operator-=(const Vector3& vector);
    Vector3 operator-() const;
    virtual Vector3 operator*(double s) const;
    Vector3 operator/(double s) const;
    Vector3& operator=(const Vector3&);
    Vector3& normalize();
    virtual double& operator[](int index);
    virtual double operator[](int index) const;
	void negate();
	void scale(double s);
	static double dot(const Vector3&, const Vector3&);
    double dot(const Vector3& b) const;
    float angle_rad(const Vector3& b) const;
    float angle(const Vector3& b) const;
	static Vector3 cross(const Vector3&, const Vector3&);
    Vector3 cross(const Vector3&) const;
	double length() const;
    double length_sq() const;
    void set(double x, double y, double z);
    double *get_pointer();
    void get_pointerf(float *f);
	
    void print(const std::string&) const;
	virtual std::string toString() const;
};

