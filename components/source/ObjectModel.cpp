#include "ObjectModel.h"
#include "Config.h"
#include "Utility.h"
#include "main.h"
#include "Window.h"
#include "ModelLoader.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static UINT32 count_lines(const char *f_name);

ObjectModel::ObjectModel()
{
    vertice_list = new VerticeList();
    normal_list = new NormalList();
    triangle_list = new TriangleList();
}

ObjectModel::ObjectModel(float *vertice, UINT64 n_vertice, float *normal,
    UINT64 n_normal, UINT32 *triangle, UINT64 n_triangle) : ObjectModel()
{
    vertice_list->set_list(vertice, n_vertice);
    normal_list->set_list(normal, n_normal);
    triangle_list->set_list(triangle, n_triangle);

    reset();
}

ObjectModel::ObjectModel(const std::string& f_name) : ObjectModel()
{
    ModelLoader::load(f_name, vertice_list, normal_list, triangle_list);

    reset();
}

ObjectModel::~ObjectModel()
{
    delete(vertice_list);
    delete(normal_list);
    delete(triangle_list);
}

TriangleList *ObjectModel::get_triangle_list()
{
    return triangle_list;
}

VerticeList *ObjectModel::get_vertice_list()
{
    return vertice_list;
}

void ObjectModel::render()
{
    material.render();

    UINT32 *indices = triangle_list->get_pointer();
    float *normals = normal_list->get_pointer();
    float *vertices = vertice_list->get_pointer();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
    glNormalPointer(GL_FLOAT, 0, normals);

    glDrawElements(GL_TRIANGLES, triangle_list->get_size() * triangle_list->get_offset(), GL_UNSIGNED_INT, indices);

    material.set_default_material();
}

void ObjectModel::reset()
{
    float *vertice;
    UINT64 n_vertices = vertice_list->get_size();
    float max_c[3], min_c[3];

    for (register UINT8 i = 0; i < 3; ++i)
    {
        max_c[i] = -10e30;
        min_c[i] = 10e30;
    }

    for (register UINT64 i = 0; i < n_vertices; ++i)
    {
        vertice = vertice_list->get_item(i);

        for (register UINT8 j = 0; j < 3; ++j)
        {
            max_c[j] = max(max_c[j], vertice[j]);
            min_c[j] = min(min_c[j], vertice[j]);
        }
    }

    scale = min(World::near_plane_width / (max_c[0] - min_c[0]), World::near_plane_height / (max_c[1] - min_c[1]));

    for (register UINT8 i = 0; i < 3; ++i)
    {
        position[i] = -scale * (max_c[i] + min_c[i]) / 2.0;
    }

    Object::updateMatrix();
}