#pragma once

#include "Sphere.h"
#include "Light.h"
#include "Material.h"

class Lamp :
    public Sphere
{
protected:
    Material material;
    Light light;
public:
    Lamp(tLightID light_ID);
    Lamp(tLightID light_ID, float radius);
    ~Lamp();

    Light *get_light_pointer();
    void render();
};

