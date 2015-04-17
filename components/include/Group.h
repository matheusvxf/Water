#pragma once
#include "Node.h"

#include <list>

class Group :
    public Node
{
protected:
    std::list<Node*> children;
public:
    Group();
    virtual ~Group();

    virtual void add_child(Node* child);
    virtual void remove_child();
    virtual void draw(const Matrix4& C);
    virtual Matrix4 get_model_view() const;
};

