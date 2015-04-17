#pragma once

#include "VerticeList.h"
#include "NormalList.h"
#include "TriangleList.h"

#include <stdio.h>
#include <string>
#include <iostream>

class ModelLoader
{
protected:
    static FILE* fp;     // file pointer
public:
    ModelLoader();
    ~ModelLoader();

    static void load(const std::string& f_name, VerticeList *vertices, NormalList *normals, TriangleList *p_face);

    /* Static variables */
    static float max_c[3];
    static float min_c[3];
};

