#include "Group.h"
#include "RenderableObject.h"
#include "Utility.h"

Group::Group()
{
}

Group::~Group()
{
    remove_child();
}

void Group::add_child(Node* child)
{
    child->set_owner(this);
    children.push_back(child);
}

void Group::remove_child()
{
    foreach(children, child)
    {
        delete (*child);
    }
    children.clear();
}

void Group::draw(const Matrix4& C)
{
    Node *node;

    foreach(children, c)
    {
        node = *c;
        node->draw(C);
    }
}

Matrix4 Group::get_model_view() const
{
    if (owner == nullptr)
        return Matrix4::matrix_identity;
    else
        return owner->get_model_view();
}