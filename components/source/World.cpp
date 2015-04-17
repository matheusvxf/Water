#include "main.h"
#include "World.h"
#include "Utility.h"
#include "MatrixTransform.h"
#include "MotionTransform.h"
#include "IUpdatable.h"
#include "Geode.h"
#include "Wall.h"
#include "Switch.h"
#include "Dragon.h"
#include "Bear.h"
#include "Lamp.h"
#include "SpotLight.h"
#include "Patch.h"
#include "Skybox.h"
#include "Texture.h"

#include <chrono>
#include <iostream>

static Plane view_frustum[6];

static GLfloat light_position[] = { 1.0, 10.0, 1.0, 0.0 };
static GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };

static const char front[] = { "resources/front.ppm" };
static const char back[] = { "resources/back.ppm" };
static const char left[] = { "resources/left.ppm" };
static const char right[] = { "resources/right.ppm" };
static const char top[] = { "resources/top.ppm" };
static const char bottom[] = { "resources/bottom.ppm" };


static const char reflection_vertex_shader[] = { "resources/shader/reflection.vert" };
static const char reflection_frag_shader[] = { "resources/shader/reflection.frag" };
static const char diffuse_vertex_shader[] = { "resources/shader/diffuse_shading.vert" };
static const char diffuse_frag_shader[] = { "resources/shader/diffuse_shading.frag" };
static const char skybox_vertex_shader[] = { "resources/shader/skybox.vert" };
static const char skybox_frag_shader[] = { "resources/shader/skybox.frag" };
static const char texture2d_vertex_shader[] = { "resources/shader/texture2D.vert" };
static const char texture2d_frag_shader[] = { "resources/shader/texture2D.frag" };
static const char vertex_shader[] = { "resources/shader/shader.vert" };
static const char frag_shader[] = { "resources/shader/shader.frag" };

double World::near_plane_height, World::near_plane_width;

Plane::Plane() {}

Plane::Plane(const Vector4& normal, const Vector4& center)
{
    this->normal = normal;
    this->center = center;
    this->normal.normalize();
}

World::World()
{
    Matrix4 translate, rotateX, *transformation;
    MatrixTransform *matrix_transform;
    Material *material;
    Texture *texture;
    Group *scene_1, *scene_2, *ocean;
    Lamp *lamp;
    Light *light;
    Shader *shader;
    Patch *patch;
    Matrix4 tmp;

    switch_model = new Switch();

    change_scene();
    calculate_frustum();

    /* Scene 1*/
    scene_1 = new Group();
    matrix_transform = new MatrixTransform(new Matrix4(Matrix4::tTranslate, Vector3(0, -0.5, 0)));
    material = new Material();
    texture = new Texture(Texture::TEXTURE_CUBE_MAP);
    material->set_property(Material::AMBIENT, 0.1, 0.4, 1.0);
    material->set_property(Material::DIFFUSE, 0.1, 0.2, 1.0);
    material->set_property(Material::SPECULAR, 0.2, 0.2, 1.0);
    material->set_property(Material::SHININESS, 32);
    texture->make_cube_map(back, front, top, bottom, left, right);
    texture->add_child(material);
    material->add_child(matrix_transform);
    matrix_transform->add_child(new Patch());
    texture->add_child(new Skybox());
    scene_1->add_child(texture);

    /* Scene 2 */
    scene_2 = new Group();
    ocean = new Group();
    texture = new Texture(Texture::TEXTURE_CUBE_MAP);
    material = material->clone();
    patch = new Patch();
    for (register UINT8 i = 0; i < 8; ++i) {
        for (register UINT8 j = 0; j < 8; ++j)
        {
            matrix_transform = new MatrixTransform();
            tmp = Matrix4(Matrix4::tTranslate, Vector3(-6 + 3 * i, 0, 3 - 3 * j));
            if (i % 2 == 0)
            {
                matrix_transform->setTransform(tmp.clone());
                matrix_transform->add_child(patch);
            }
            else
            {
                tmp *= Matrix4(Matrix4::tRotate, 180, 0, 1, 0);
                tmp *= Matrix4(Matrix4::tRotate, 0, 1, 0, 0);
                matrix_transform->setTransform(tmp.clone());
                matrix_transform->add_child(patch);
            }
            ocean->add_child(matrix_transform);
        }
    }
    texture->make_cube_map(back, front, top, bottom, left, right);
    texture->add_child(material);
    material->add_child(ocean);
    ocean->add_child(matrix_transform);
    scene_2->add_child(texture);

    switch_model->add_child(scene_1, 0x01);
    switch_model->add_child(scene_2, 0x02);
    switch_model->add_child(new Bear(), 0x04);
    switch_model->set_mask(0x01);
    add_child(switch_model);

    /* Lights */
    /* Lamp */
    translate.makeTranslate(0, 2, 0);
    transformation = translate.clone();
    matrix_transform = new MatrixTransform(transformation);
    lamp = new Lamp(GL_LIGHT1, 1);
    light = lamp->get_light_pointer();
    light->set_property(Light::DIFFUSE, Color(ORANGE));
    light->set_property(Light::SPECULAR, Color(ORANGE));
    matrix_transform->add_child(lamp);
    add_child(matrix_transform);
}

World::~World() {}

void World::change_scene()
{
    Globals::camera->set_center_of_proj(Vector4(0, 1, 2.5));
    Globals::camera->look_at(Vector4(0, 0, 1));
    Globals::camera->set_up(Vector4(0, 1, 0));

    switch_model->set_mask(Globals::scene);
}

Node *World::get_curr_object()
{
    if (Globals::control_light)
        return spot_light;
    else
        return switch_model->get_first_element();
}

Node *World::get_model()
{
    return switch_model->get_first_element();
}

void World::draw(const Matrix4& C)
{
    GL_DEBUG("World::draw - start");

    glLightfv(GL_LIGHT7, GL_POSITION, light_position);
    glLightfv(GL_LIGHT7, GL_SPECULAR, light_ambient);
    glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT7);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GL_DEBUG("World:: - setup");

    Group::draw(C);

    GL_DEBUG("World::draw - end");
}

void World::update()
{
    IUpdatable::call_update();
}

BOOLEAN World::is_in_view_frostum(const Node& object)
{
    static Vector4 center;
    double distance;
    double distance_module;
    double radius;
    for (register UINT8 i = 0; i < 6; ++i)
    {
        radius = object.get_bounding_sphere_radius();
        center = object.get_bounding_sphere_center();
        distance = distance2plane(view_frustum[i], center);
        distance_module = distance > 0 ? distance : -distance;
        if (distance > radius)
            return FALSE;
    }
    return TRUE;
}

void World::calculate_frustum()
{
    double dx, dy, x, y, z, FOV_w;
    Matrix4 rotX, rotY;
    Vector4 normal;

    near_plane_height = 2.0 * Globals::near_plane * tan(degree2rad(Globals::FOV / 2.0));
    near_plane_width = Globals::aspect * near_plane_height;
    z = -(Globals::near_plane + Globals::far_plane) / 2.0;
    FOV_w = 2 * rad2degree(atan2(near_plane_width / 2.0, Globals::near_plane));
    dy = tan(degree2rad(Globals::FOV / 2.0)) * (-z - Globals::near_plane);
    dx = tan(degree2rad(FOV_w / 2.0)) * (-z - Globals::near_plane);

    /* Near plane */
    view_frustum[0] = Plane(Vector4(0, 0, 1), Vector4(0, 0, -Globals::near_plane));
    /* Far plane */
    view_frustum[1] = Plane(Vector4(0, 0, -1), Vector4(0, 0, -Globals::far_plane));
    /* Right plane */
    rotY.makeRotateY(-FOV_w / 2.0);
    normal.set(1, 0, 0, 0);
    normal = rotY * normal;  
    y = 0;
    x = near_plane_width / 2.0 + dx;
    view_frustum[2] = Plane(normal, Vector4(x, y, z));
    /* Left plane */
    rotY.makeRotateY(FOV_w / 2.0);
    normal.set(-1, 0, 0, 0);
    normal = rotY * normal;
    y = 0;
    x = -x;
    view_frustum[3] = Plane(normal, Vector4(x, y, z));
    /* Up plane */
    y = near_plane_height /2.0 + dy;
    normal.set(0, 1, 0, 0);
    rotX.makeRotateX(Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[4] = Plane(normal, Vector4(0, y, z));
    /* Down plane */
    y = -y;
    normal.set(0, -1, 0, 0);
    rotX.makeRotateX(-Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[5] = Plane(Vector4(normal), Vector4(0, y, z));
}