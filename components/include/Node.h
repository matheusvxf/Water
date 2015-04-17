#pragma once

#include "Matrix4.h"
#include "Vector3.h"

class Node
{
protected:
    /* Bounding sphere */
    double bounding_sphere_radius;
    Vector3 bounding_sphere_center;
    Vector3 bounding_box[3];

    Node *owner;
public:
    Node();
    virtual ~Node();

    virtual void draw(const Matrix4& C) = 0;
    virtual Matrix4 get_model_view() const = 0;
    void draw_bounding_sphere();
    void set_bouding_box(const Vector3&a, const Vector3& b, const Vector3& c);
    void set_owner(Node *owner);
    const Vector3& get_bounding_sphere_center() const;
    double get_bounding_sphere_radius() const;
};
