#include "Lamp.h"

#include "GLee.h"

static float default_position[] = { 0.0, 0.0, 0.0, 1.0 };

Lamp::Lamp(tLightID light_ID) : Lamp(light_ID, 1.0) {}

Lamp::Lamp(tLightID light_ID, float radius) : light(light_ID, default_position), Sphere(radius)
{
    material.enable_property(Material::EMISSION);
    material.set_property(Material::EMISSION, Color(ORANGE).apply_attenuation(0.8));
}

Lamp::~Lamp()
{
}

void Lamp::render()
{
    material.render();
    light.render();
    Sphere::render();
    material.set_default_material();
}

Light *Lamp::get_light_pointer()
{
    return &light;
}