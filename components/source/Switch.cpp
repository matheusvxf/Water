#include "Switch.h"
#include "Utility.h"

Switch::Switch()
{
    mask = 0xffffffffffffffff;
}

Switch::~Switch()
{
}

void Switch::set_mask(UINT64 mask)
{
    this->mask = mask;
}

void Switch::add_group(UINT64 group_id)
{
    map[group_id];
}

Node *Switch::get_first_element()
{
    return get_first_list()->front();
}

std::list<Node*> *Switch::get_first_list()
{
    foreach(map, it)
    {
        if (it->first & mask)
        {
            return &it->second;
        }
    }
    return nullptr;
}

void Switch::add_child(Node *child, UINT64 group_id)
{
    Group::add_child(child);

    auto it = map.find(group_id);
    
    if (it == map.end())
    {
        add_group(group_id);
    }

    map[group_id].push_back(child);
}

void Switch::draw(const Matrix4& C)
{
    auto it = map.find(mask);

    if (it == map.end())
        return;

    foreach(it->second, l)
    {
        (*l)->draw(C);
    }
}