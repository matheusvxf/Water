#pragma once

#include "Group.h"

#include <map>
#include <list>

class Switch :
    public Group
{
protected:
    std::map<UINT64, std::list<Node*> > map;
    UINT64 mask;
public:
    Switch();
    virtual ~Switch();

    void set_mask(UINT64 mask);
    void add_group(UINT64 group_id);
    Node *get_first_element();
    std::list<Node*> *get_first_list();
    virtual void add_child(Node *child, UINT64 group_id);
    virtual void draw(const Matrix4& C);
};

