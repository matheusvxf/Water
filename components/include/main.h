#ifndef _MAIN_H_
#define _MAIN_H_

#include "GLee.h"
#include "Camera.h"
#include "World.h"
#include "Shader.h"

enum tScene
{
    tBunny = 0x01,
    tDragon = 0x02,
    tBear = 0x04
};

namespace Globals
{
    extern Camera* camera;
    extern Shader* shader;
    extern World* world;
    extern Object **curr_obj;
    extern BOOLEAN draw_bounding_sphere;
    extern BOOLEAN control_light;
    extern BOOLEAN per_pixel_lighting;
    extern BOOLEAN culling;
    extern BOOLEAN geometry_debug;
    extern BOOLEAN environment_mapping;
    extern double FOV;
    extern double near_plane;
    extern double far_plane;
    extern double aspect;
    extern tScene scene;
    extern GLuint vertexShaderObject, fragmentShaderObject;
};

#endif