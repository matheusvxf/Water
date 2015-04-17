#pragma once

#include "DataType.h"
#include "Color.h"
#include "Group.h"

class Material :
    public Group
{
public:
    enum tMaterialType
    {
        AMBIENT = 0x01,
        DIFFUSE = 0x02,
        SPECULAR = 0x04,
        EMISSION = 0x08,
        SHININESS = 0x10
    };
protected:
    Color ambient;
    Color diffuse;
    Color specular;
    Color emission;
    float shininess[1];

    tMaterialType mask;
public:

    Material();
    Material* clone();
    virtual ~Material();

    void enable_property(tMaterialType type);
    void set_property(tMaterialType type, const Color& c);
    void set_property(tMaterialType type, float r, float g, float b);
    void set_property(tMaterialType type, float);
    void unset_material(tMaterialType type);
    BOOLEAN has_material_type(tMaterialType type);

    void draw(const Matrix4& C);
    void render();
    void set_default_material();
};

