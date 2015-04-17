#include "Node.h"


Node::Node() : bounding_sphere_radius(1), bounding_sphere_center(0, 0, 0), owner(nullptr) {}

Node::~Node() {}

void Node::draw_bounding_sphere()
{
    static Matrix4 translate;
    translate.makeTranslate(bounding_sphere_center[0], bounding_sphere_center[1], bounding_sphere_center[2]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glLoadMatrixd(translate.transpose().getPointer());
        glutWireSphere(bounding_sphere_radius, 16, 16);
    glPopMatrix();
}

void Node::set_bouding_box(const Vector3&a, const Vector3& b, const Vector3& c)
{
    bounding_box[0] = a;
    bounding_box[1] = b;
    bounding_box[2] = c;
}

void Node::set_owner(Node *owner)
{
    this->owner = owner;
}

const Vector3& Node::get_bounding_sphere_center() const
{
    return bounding_sphere_center;
}

double Node::get_bounding_sphere_radius() const
{
    return bounding_sphere_radius;
}