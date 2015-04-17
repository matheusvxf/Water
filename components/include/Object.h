#pragma once

#include "Vector4.h"
#include "Matrix4.h"
#include "IUpdatable.h"

class Object :
    public IUpdatable
{
public:
    enum ObjectType
    {
        tCube,
        tBall,
        tHouse
    };
    ObjectType obj_type;
protected:
    double angleX;
    double angleY;                   // rotation angle [degrees]
    double angleZ;
    double scale;

    Matrix4 model2world;            // model matrix (transforms model coordinates to world coordinates)
    Matrix4 rotate_axis;
    Vector4 position;
    Vector4 velocity;
    Vector4 acceleration;
    Vector4 force;
    double mass;
    double friction;

public:
    Object();
    virtual ~Object();

    virtual void render();

    virtual void update();
    virtual void reset();
    virtual void print();
    virtual std::string toString();

    Matrix4& getMatrix();
    Matrix4& get_model2world();
    Vector4& getPosition();
    double getSpeed() const;
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    void rotate(float angle, float x, float y, float z);
    void rotate(float angle, const Vector3& a);
    void clean_rotation();
    void addScale(double scale);
    void updateMatrix();
    void apply_force(const Vector3& force);
    void apply_acceleration(const Vector3& acceleration);
};

