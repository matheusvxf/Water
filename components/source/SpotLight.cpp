#include "SpotLight.h"

static float default_position[] = { 0.0, 0.0, 0.0, 1.0 };
static float default_direction[] = { 0.0, 0.0, -1.0 };
static float default_cutoff = 15.0;
static float default_exponent = 4.0;

SpotLight::SpotLight(tLightID light_ID) : SpotLight(GL_LIGHT0, 1.0, 1.0) {}

SpotLight::SpotLight(tLightID light_ID, float base, float height) : SpotLight(light_ID, base, height, default_cutoff, default_exponent) {}

SpotLight::SpotLight(tLightID light_ID, float base, float height, float cutoff, float exponent) : 
    SpotLight(light_ID, base, height, cutoff, exponent, default_position, default_direction) {}

SpotLight::SpotLight(tLightID light_ID, float base, float height, float cutoff, float exponent, float position[], float direction[]) : 
    Cone(base, height), light(light_ID, position, direction)
{
    float d[] = { 0, -1, 0 };
    light.set_direction(d);
    light.set_property(Light::tLightType::SPOT_CUTOFF, cutoff);
    light.set_property(Light::tLightType::SPOT_EXPONENT, exponent);
    light.set_property(Light::tLightType::DIFFUSE, Color(0.1, 0.1, 0.8));
    light.set_property(Light::tLightType::SPECULAR, Color(0.0, 0.01, 0.8));
    material.set_property(Material::EMISSION, Color(BLUE));
    material.set_property(Material::DIFFUSE, Color(0.2, 0.2, 0.2));
}

SpotLight::~SpotLight() {}

void SpotLight::draw(const Matrix4 &C)
{
    light.draw(C * model2world);
    Geode::draw(C);
}

void SpotLight::render()
{
    material.render();
    Cone::render();
    material.set_default_material();
}

void SpotLight::update()
{
    Object::update();
}

Light *SpotLight::get_light_pointer()
{
    return &light;
}

float *SpotLight::get_direction()
{
    float *direction;
    light.get_property(Light::tLightType::SPOT_DIRECTION, &direction);
    return direction;
}

void SpotLight::set_direction(Vector3& v)
{
    float f[3];
    v.get_pointerf(f);
    light.set_direction(f);
}