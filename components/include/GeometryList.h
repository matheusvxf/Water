#pragma once

#include "DataType.h"

#include <list>

template<class T>
class GeometryList
{
protected:
    BOOLEAN dynamic_memory;
    UINT64 n_elements;
    UINT32 offset;
    T *array;
public:
    GeometryList();
    virtual ~GeometryList();

    void set_list(std::list<T*> *p_list);
    void set_list(T *p_list, UINT64 size);
    UINT64 get_size() const;
    UINT32 get_offset() const;
    T *get_pointer();
    inline T *get_item(UINT64 i);
};

template< class T>
GeometryList<T>::GeometryList() : array(nullptr), n_elements(0), offset(1), dynamic_memory(FALSE) {}

template< class T>
GeometryList<T>::~GeometryList()
{
    if (dynamic_memory && array != nullptr)
        delete[]array;
}

template< class T>
UINT64 GeometryList<T>::get_size() const
{
    return n_elements / offset;
}

template< class T>
UINT32 GeometryList<T>::get_offset() const
{
    return offset;
}

template< class T>
T *GeometryList<T>::get_pointer()
{
    return array;
}

template <class T>
void GeometryList<T>::set_list(std::list<T*> *p_list)
{
    UINT32 n_elements = p_list->size();
    T *v_p;

    if (this->array != nullptr)
        delete[]this->array;

    this->n_elements = n_elements;
    this->array = (T*)malloc(n_elements * offset * sizeof(T));

    for (register UINT32 i = 0; i < n_elements; ++i)
    {
        v_p = p_list->front();
        for (register UINT32 j = 0; j < offset; ++j)
        {
            this->array[i * offset + j] = v_p[j];
        }
        p_list->pop_front();
        delete(v_p);
    }
}

template <class T>
void GeometryList<T>::set_list(T *list, UINT64 size)
{
    array = list;
    n_elements = size;
    dynamic_memory = FALSE;
}

template <class T>
T *GeometryList<T>::get_item(UINT64 i)
{
    return array + i * offset;
}