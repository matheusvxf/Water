#pragma once

#include "Geode.h"
#include "Color.h"
#include "DataType.h"

typedef UINT16 tLightID;

class Light :
    public Node
{
public:
    enum tLightType
    {
        AMBIENT = 0x01,
        DIFFUSE = 0x02,
        SPECULAR = 0x04,
        DIRECTIONAL = 0x08,
        SPOTLIGHT = 0x10,
        SPOT_CUTOFF = 0x30,
        SPOT_EXPONENT = 0x50,
        SPOT_DIRECTION = 0x90,
        POINT = 0x100,

    };
protected:
    tLightID light_ID;
    float position[4];
    float direction[3];
    Color ambient;
    Color diffuse;
    Color speculer;

    float spot_cutoff;
    float spot_exponent;
    
    tLightType mask;
public:
    Light();
    Light(tLightID light_ID);
    Light(tLightID light_ID, float position[]);
    Light(tLightID light_ID, float position[], float direction[]);
    virtual ~Light();

    void set_position(float position[]);
    void set_direction(float direction[]);
    void enable_property(tLightType type);
    void set_property(tLightType type, const Color& c);
    void set_property(tLightType type, float p);
    float get_property(tLightType type);
    void get_property(tLightType type, float **value);
    void unset_property(tLightType type);
    BOOLEAN has_property(tLightType type);
    void draw(const Matrix4& C);
    Matrix4 get_model_view() const;
    void render();
};

