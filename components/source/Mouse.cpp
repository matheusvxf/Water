#include "Mouse.h"
#include "main.h"
#include "Vector4.h"
#include "Config.h"
#include "Utility.h"
#include "Window.h"
#include "ObjectModel.h"

#include <iostream>

static Vector4 last_mouse_pos;
static BOOLEAN first_click = TRUE;

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

/*******************************************************************************
    The CPoint class is a specific Windows class. Either use separate x and y
    values for the mouse location, or use a Vector3 in which you ignore the z
    coordinate.
*******************************************************************************/
Vector3 track_ball_mapping(const Vector3& point)
{
    Vector3 v;  /* Vector v is the synthesized 3D position of the mouse location 
                on the trackball */
    float d;    /* this is the depth of the mouse location: the delta between the
                plane through the center of the trackball and the z position of
                the mouse */
    
    /* this calculates the mouse X position in trackball coordinates, which range from -1 to +1 */
    v[0] = (2.0 * point[0] - Window::width) / Window::width;
    /* this does the equivalent to the above for the mouse Y position */
    v[1] = (Window::height - 2.0 * point[1]) / Window::height;
    /* initially the mouse z position is set to zero, but this will change below */
    v[2] = 0.0;
    
    /* this is the distance from the trackball's origin to the mouse location,
        without considering depth (=in the plane of the trackball's origin) */
    d = v.length();
    /* this limits d to values of 1.0 or less to avoid square roots of negative
        values in the following line */
    d = (d < 1.0) ? d : 1.0;
    /* this calculates the Z coordinate of the mouse position on the trackball,
        based on Pythagoras: v.z*v.z + d*d = 1*1 */
    v[2] = sqrtf(1.001 - d*d);
    v.normalize();  // Still need to normalize, since we only capped d, not v.
    return v;       // return the mouse location on the surface of the trackball
}

void map(INT32 *x, INT32 *y)
{
    *x = *x - Window::width / 2.0;
    *y = *y - Window::height / 2.0;
    *y = -1 * *y;
}

float* find_intersection(INT32 x, INT32 y)
{
    ObjectModel *object;
    TriangleList *triangle;
    VerticeList *vertice;
    UINT32 *p_triangle_index;
    Matrix4 model2world, model2camera;
    float v0[3], v1[3], v2[3], direction[3], point[3], *closer_point;
    Vector4 v_0, v_1, v_2, closer;
    float z_closer;
    BOOLEAN found;

    found = false;
    closer_point = (float*)malloc(sizeof(float)* 3);

    object = dynamic_cast<ObjectModel*>(Globals::world->get_model());

    if (object != nullptr)
    {
        triangle = object->get_triangle_list();
        vertice = object->get_vertice_list();
        model2world = object->get_model_view();
        model2camera = Globals::camera->getMatrix() * model2world;
        map(&x, &y);
        
        z_closer = -Globals::far_plane;

        direction[0] = (x + Window::width / 2.0) / Window::width * World::near_plane_width - World::near_plane_width / 2.0;
        direction[1] = (y + Window::height / 2.0) / Window::height * World::near_plane_height - World::near_plane_height / 2.0;
        direction[2] = -Globals::near_plane;
        point[0] = 0;
        point[1] = 0;
        point[2] = 0;

        for (register UINT64 i = 0; i < triangle->get_size(); ++i)
        {
            p_triangle_index = triangle->get_item(i);

            v_0 = Vector4(vertice->get_item(p_triangle_index[0]), 1);
            v_1 = Vector4(vertice->get_item(p_triangle_index[1]), 1);
            v_2 = Vector4(vertice->get_item(p_triangle_index[2]), 1);
            v_0 = model2camera * v_0;
            v_1 = model2camera * v_1;
            v_2 = model2camera * v_2;
            v_0.get_pointerf(v0);
            v_1.get_pointerf(v1);
            v_2.get_pointerf(v2);
            if (rayIntersectsTriangle(point, direction, v0, v1, v2))
            {
                if (z_closer <= v_0[2] && v_0[2] <= 0)
                {
                    z_closer = v_0[2];
                    found = TRUE;
                    closer = v_0;
                }
            }
        }
    }
    closer_point[0] = closer[0];
    closer_point[1] = closer[1];
    closer_point[2] = closer[2];
    if (found)
    {
        return closer_point;
    }
    else
    {
        delete[] closer_point;
        return nullptr;
    }
}

void change_direction(INT32 x, INT32 y)
{
    float *p_point = find_intersection(x, y);
    float angle;
    Vector4 point;
    SpotLight *spot_light = dynamic_cast<SpotLight*>(Globals::world->get_curr_object());
    Matrix4 model2world, model2camera, test;
    Vector3 a, direction, new_direction;
    Vector4 position;

    if (spot_light == nullptr || p_point == nullptr)
        return;
    
    spot_light->clean_rotation();

    point = Vector4(p_point, 1.0);
    model2world = spot_light->get_model_view();
    model2camera = Globals::camera->getMatrix() * model2world;

    position = spot_light->getPosition();
    position = model2camera * position;
    direction = Vector3(spot_light->get_direction());
    direction = model2camera * direction;
    new_direction = point - position;
    position.print("position");
    direction.print("direction");
    new_direction.print("new direction");
    a = new_direction.cross(direction);
    a.print("a");
    angle = direction.angle(new_direction);
    std::cout << "angle : " << angle << std::endl;

    if (abs(angle) > 0.00001)
    {
        test.makeRotate(angle, a);
        (test * direction).print("new");
        spot_light->set_direction(new_direction);
        spot_light->rotate(angle, a);
        test = Globals::camera->getMatrix() * spot_light->get_model_view();
        direction = Vector3(spot_light->get_direction());
    }
}

void Mouse::leftMotion(int x, int y)
{
    static Vector3 old_location;
    Vector3 location, a;
    Vector4 p;
    Matrix4 transform;
    float angle;

    location = track_ball_mapping(Vector3(x, y, 0));

    if (first_click)
    {
        first_click = FALSE;
    }
    else
    {
        a = old_location.cross(location);

        angle = location.angle(old_location);

        if (abs(angle) > 0.0001)
        {
            transform.makeRotate(angle, a);

            Globals::camera->rotate(angle, a);
            Globals::camera->calculate_camera_matrix();
            /*p = Globals::camera->look_at();
            p = transform * p;
            Globals::camera->look_at(p);
            p = Globals::camera->get_up();
            p = transform * p;
            Globals::camera->set_up(p);*/

            /*object = dynamic_cast<Object*>(Globals::world->get_curr_object());

            if (object != nullptr)
            {
            object->rotate(angle, a);
            }*/
        }
    }

    old_location = location;
}

void Mouse::rightMotion(int x, int y)
{
    static Vector3 old_location, location;
    Vector4 center, look_at, v;
    float cutoff;

    location = track_ball_mapping(Vector3(x, y, 0));

    if (first_click)
    {
        first_click = FALSE;
    }
    else
    {
        center = Globals::camera->center_of_proj();
        look_at = Globals::camera->look_at();
        v = look_at - center;
        v.normalize();
        cutoff = (location - old_location).length_sq() * 2;
        cutoff = location[1] < old_location[1] ? -cutoff : cutoff;
        center += v * cutoff;
        Globals::camera->center_of_proj(center);

        cutoff = location[0] - old_location[0];
        look_at = Matrix4(Matrix4::tRotate, cutoff * 100, 0, 1, 0) * look_at;
        Globals::camera->look_at(look_at);

        /*object = dynamic_cast<Object*>(Globals::world->get_curr_object());

        if (object != nullptr)
        {
            if (location[1] < old_location[1])
                object->addScale(-(location - old_location).length_sq());
            else
                object->addScale((location - old_location).length_sq());
        }*/
    }

    old_location = location;
}

void Mouse::middleMotion(int x, int y)
{
}

void Mouse::mouseCallback(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        /* A button is being pressed. Set the correct motion function */
        if (button == GLUT_LEFT_BUTTON)
        {
            if (Globals::control_light == FALSE)
                glutMotionFunc(leftMotion);
            else
                change_direction(x, y);
        }
        else if (button == GLUT_RIGHT_BUTTON)
            glutMotionFunc(rightMotion);
    }
    else if (state == GLUT_UP)
    {
        glutMotionFunc(nullptr);
        first_click = TRUE;
    }
}