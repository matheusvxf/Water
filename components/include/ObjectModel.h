#pragma once

#include "Geode.h"
#include "VerticeList.h"
#include "NormalList.h"
#include "TriangleList.h"
#include "Material.h"

#include "GL/glut.h"

class ObjectModel :
    public Geode
{
protected:
    Material material;
    VerticeList *vertice_list;
    NormalList *normal_list;
    TriangleList *triangle_list;
    float max[3], min[3];
    
public:
    ObjectModel();
    ObjectModel(float *vertice, UINT64 n_vertice, float *normal, UINT64 n_normal, UINT32 *triangle, UINT64 n_triangle);
    ObjectModel(const std::string& f_name);
    ~ObjectModel();

    TriangleList *get_triangle_list();
    VerticeList *get_vertice_list();
    BOOLEAN load_file(const char *f_name);
    virtual void render();
    virtual void reset();
};