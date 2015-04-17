#include "IUpdatable.h"
#include "Utility.h"

std::list<IUpdatable*> IUpdatable::list;

IUpdatable::IUpdatable()
{
    list.push_back(this);
}


IUpdatable::~IUpdatable()
{
    list.remove(this);
}

void IUpdatable::call_update()
{
    foreach (list, it)
    {
        (*it)->update();
    }
}
