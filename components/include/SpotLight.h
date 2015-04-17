#pragma once

#include "Cone.h"
#include "Material.h"
#include "Light.h"

class SpotLight :
    public Cone
{
protected:
    Material material;
    Light light;

public:
    SpotLight();
    SpotLight(tLightID light_ID);
    SpotLight(tLightID light_ID, float base, float height);
    SpotLight(tLightID light_ID, float base, float height, float cutoff, float exponent);
    SpotLight(tLightID light_ID, float base, float height, float cutoff, float exponent, float position[], float direction[]);
    virtual ~SpotLight();

    virtual void draw(const Matrix4 &C);
    virtual void render();
    virtual void update();
    Light *get_light_pointer();
    float *get_direction();
    void set_direction(Vector3&);
};

