#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "DataType.h"
#include "Vector3.h"
#include "Vector4.h"

#include <string>
#include <GL/glut.h>

class Matrix4
{
public:
    enum MatrixType
    {
        tIdentity,
        tTranslate,
        tRotate
    };

    const static Matrix4 matrix_identity;
private:
    class Matrix4row
    {
    private:
        Matrix4* owner;
        UINT8 row;
        
    public:
        Matrix4row(Matrix4* owner, UINT8 row);
        Matrix4row& operator=(const Vector4& vector);
        double operator[](UINT8 column) const;
        double& operator[](UINT8 column);
    };
protected:
    double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)

public:
    Matrix4();
    Matrix4(const Matrix4& m);
    Matrix4(MatrixType type);
    Matrix4(MatrixType type, const Vector3& v);
    Matrix4(MatrixType type, double x, double y, double z);
    Matrix4(MatrixType type, double angle, double x, double y, double z);
    Matrix4(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4);
    Matrix4* clone();

    Matrix4& operator=(const Matrix4&);
    Matrix4& operator=(GLdouble *matrix);
    Matrix4 operator*(const Matrix4& m2) const;
    Matrix4& operator*=(const Matrix4& m);
    Vector4 operator*(const Vector4& v) const;
    Matrix4 Matrix4::operator*(double s) const;
    Matrix4& Matrix4::operator*=(double s);
    Matrix4 operator-() const;
    Matrix4row operator[](UINT8 row);
    void setColumn(UINT8 column, const Vector4& v);
    void setColumn(UINT8 column, const Vector3& v);
    inline void set(UINT8 i, UINT8 j, double value) { m[i][j] = value; }
    const double* getPointer() const;
    void get_pointerf(float *m) const;
    inline double get(UINT8 i, UINT8 j) const { return m[i][j]; }
    void identity();
    Matrix4& transpose();
    Matrix4& make_skew(const Vector3& v);
    Matrix4& makeRotateX(double angle);
    Matrix4& makeRotateY(double angle);
    Matrix4& makeRotateZ(double angle);
    Matrix4& makeRotate(double angle, const Vector3& axis);
    Matrix4& makeRotate(double angle, double x, double y, double z);
    Matrix4& makeScale(double sx, double sy, double sz);
    Matrix4& makeScale(double s);
    Matrix4& makeTranslate(double sx, double sy, double sz);

    void print(const std::string& s) const;
    std::string toString() const;
};

#endif