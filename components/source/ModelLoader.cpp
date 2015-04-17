#include "ModelLoader.h"
#include "DataType.h"

#include <iostream>
#include <list>
#include <algorithm>

float ModelLoader::max_c[3];
float ModelLoader::min_c[3];
FILE *ModelLoader::fp = new FILE;

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::load(const std::string& f_name, VerticeList *p_vertices, NormalList *p_normals, TriangleList *p_face)
{
    int c1, c2;     // characters read from file
    std::list<float*> *vertice;
    std::list<float*> *normal;
    std::list<UINT32*> *triangle;
    float *v;
    UINT32 *f;

    for (UINT8 i = 0; i < 3; ++i)
    {
        min_c[i] = (float)10e30;
        max_c[i] = (float)-10e30;
    }

    vertice = new std::list<float*>;
    normal = new std::list<float*>;
    triangle = new std::list<UINT32*>;

    fopen_s(&fp, f_name.c_str(), "rb");

    if (fp == NULL)
    {
        std::cerr << "error loading file" << std::endl;
        exit(-1);
    }

    c1 = fgetc(fp);
    c2 = fgetc(fp);

    while(c1 != EOF && c2 != EOF)
    {
        if ((c1 == 'v') && (c2 == ' '))
        {
            v = new float[6];
            fscanf_s(fp, "%f %f %f %f %f %f", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
            for (register UINT8 i = 0; i < 3; ++i)
            {
                max_c[i] = max(max_c[i], v[i]);
                min_c[i] = min(min_c[i], v[i]);
            }
            vertice->push_back(v);
        }
        else if ((c1 == 'v') && (c2 == 'n'))
        {
            v = new float[3];
            fscanf_s(fp, "%f %f %f %f %f %f", &v[0], &v[1], &v[2]);
            normal->push_back(v);
        }
        else if ((c1 == 'f') && (c2 == ' '))
        {
            f = new UINT32[6];
            fscanf_s(fp, "%d//%d %d//%d %d//%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
            for (register UINT8 i = 0; i < 6; ++i)
                f[i]--;
            triangle->push_back(f);
        }
        else if ((c1 == '#') || (c1 == ' '))
        {
            while (c1 != '\n' && c1 != EOF)
                c1 = fgetc(fp);
        }
        
        c1 = fgetc(fp);
        c2 = fgetc(fp);
    }
    p_vertices->set_list(vertice);
    p_normals->set_list(normal);
    p_face->set_list(triangle);

    fclose(fp);   // make sure you don't forget to close the file when done
}