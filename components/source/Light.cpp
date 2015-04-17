#include "Light.h"
#include "main.h"

static float default_position[] = { 0.0, 0.0, 0.0, 1.0 };
static float default_direction[] = { 0.0, 0.0, 1.0 };
static Color default_ambient(0.0, 0.0, 0.0, 1.0);
static Color default_diffuse(1.0, 1.0, 1.0, 1.0);
static Color default_specular(1.0, 1.0, 1.0, 1.0);
static float default_spot_cutoff = 180.0;
static float default_spot_exponent = 0.0;

Light::Light() : Light(GL_LIGHT0) {}

Light::Light(tLightID light_ID) : Light(light_ID, default_position) {}

Light::Light(tLightID light_ID, float position[]) : Light(light_ID, position, default_direction) {}

Light::Light(tLightID light_ID, float position[], float direction[]) :
    light_ID(light_ID), mask(AMBIENT)
{
    set_position(position);
    set_direction(direction);
    ambient = default_ambient;
    diffuse = default_diffuse;
    speculer = default_specular;
    spot_cutoff = default_spot_cutoff;
    spot_exponent = default_spot_exponent;
}

Light::~Light()
{
    glDisable(light_ID);
}

void Light::set_position(float position[])
{
    for (register UINT8 i = 0; i < 4; ++i)
        this->position[i] = position[i];
}

void Light::set_direction(float direction[])
{
    for (register UINT8 i = 0; i < 3; ++i)
        this->direction[i] = direction[i];
}

void Light::enable_property(tLightType type)
{
    mask = (tLightType)(mask | type);
}

void Light::set_property(tLightType type, const Color& c)
{
    enable_property(type);
    switch (type)
    {
    case tLightType::AMBIENT:
        ambient = c;
        break;
    case tLightType::DIFFUSE:
        diffuse = c;
        break;
    case tLightType::SPECULAR:
        speculer = c;
        break;
    }
}

void Light::set_property(tLightType type, float p)
{
    enable_property(type);
    switch (type)
    {
    case tLightType::SPOT_CUTOFF:
        spot_cutoff = p;
        break;
    case tLightType::SPOT_EXPONENT:
        spot_exponent = p;
        break;
    }
}

float Light::get_property(tLightType type)
{
    switch (type)
    {
    case tLightType::SPOT_CUTOFF:
        return spot_cutoff;
    case tLightType::SPOT_EXPONENT:
        return spot_exponent;
    }
    return 0.0;
}

void Light::get_property(tLightType type, float **value)
{
    switch (type)
    {
    case tLightType::SPOT_DIRECTION:
        *value = direction;
        break;
    }
}

void Light::unset_property(tLightType type)
{
    mask = (tLightType)(mask & ~(mask & type));
}

BOOLEAN Light::has_property(tLightType type)
{
    return mask & type ? TRUE : FALSE;
}

void Light::draw(const Matrix4& C)
{
    static Matrix4 transformation;
    transformation = Globals::camera->getMatrix() * C;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(transformation.transpose().getPointer());
    render();
}

Matrix4 Light::get_model_view() const
{
    if (owner == nullptr)
        return Matrix4::matrix_identity;
    else
        return owner->get_model_view();
}

void Light::render()
{
    glLightfv(light_ID, GL_POSITION, position);

    if (mask & tLightType::AMBIENT)
    {
        glLightfv(light_ID, GL_AMBIENT, ambient.get_pointer());
    }
    if (mask & tLightType::DIFFUSE)
    {
        glLightfv(light_ID, GL_DIFFUSE, diffuse.get_pointer());
    }
    if (mask & tLightType::SPECULAR)
    {
        glLightfv(light_ID, GL_SPECULAR, speculer.get_pointer());
    }
    if (mask & tLightType::SPOTLIGHT)
    {
        glPushMatrix();
        glLoadIdentity();
        glLightf(light_ID, GL_SPOT_EXPONENT, float(spot_exponent));
        glLightf(light_ID, GL_SPOT_CUTOFF, spot_cutoff);
        glLightfv(light_ID, GL_SPOT_DIRECTION, direction);
        glPopMatrix();
    }

    glEnable(light_ID);
}