#pragma once

#include "Vector4.h"
#include "Geode.h"
#include "Group.h"
#include "Switch.h"
#include "SpotLight.h"
#include "Shader.h"

#include <vector>

class Plane
{
public:
    Plane();
    Plane(const Vector4& normal, const Vector4& center);
	Vector4 normal;
	Vector4 center;
};

class World :
    Group
{
protected:
    Switch *switch_model;
    SpotLight *spot_light;
    Shader *reflection;
public:
    World();
    ~World();

    void init_spot_light();
    void change_scene();
    Node *get_curr_object();
    Node *get_model();

    void draw(const Matrix4& C);
    void update();
    static void calculate_frustum();
    static BOOLEAN is_in_view_frostum(const Node& object); 

    static double near_plane_height, near_plane_width;
    Matrix4 *mouse_transformation;
};