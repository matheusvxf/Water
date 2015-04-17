#include "Material.h"

#include "GLee.h"

static Color default_ambient(0.2f, 0.2f, 0.2f, 1.0f);
static Color default_diffuse(0.8f, 0.8f, 0.8f, 1.0f);
static Color default_specular(0.0f, 0.0f, 0.0f, 1.0f);
static Color default_emission(0.0f, 0.0f, 0.0f, 1.0f);
static float default_shininess[] = { 0.0 };

Material::Material()
{
    ambient = default_ambient;
    diffuse = default_diffuse;
    specular = default_specular;
    emission = default_emission;
    shininess[0] = default_shininess[0];
    mask = (tMaterialType)(0x00);
}

Material* Material::clone()
{
    Material *material = new Material();

    material->ambient = ambient;
    material->diffuse = diffuse;
    material->specular = specular;
    material->emission = emission;
    material->shininess[0] = shininess[0];
    material->mask = mask;

    return material;
}

Material::~Material() {}

void Material::enable_property(tMaterialType type)
{
    mask = (tMaterialType)(mask | type);
}

void Material::set_property(tMaterialType type, const Color& c)
{
    enable_property(type);
    switch (type)
    {
    case tMaterialType::AMBIENT:
        ambient = c;
        break;
    case tMaterialType::DIFFUSE:
        diffuse = c;
        break;
    case tMaterialType::SPECULAR:
        specular = c;
        break;
    case tMaterialType::EMISSION:
        emission = c;
        break;
    }
}

void Material::set_property(tMaterialType type, float r, float g, float b)
{
    return set_property(type, Color(r, g, b));
}

void Material::set_property(tMaterialType flag, float p)
{
    switch (flag)
    {
    case tMaterialType::SHININESS:
        shininess[0] = p;
        break;
    }
}

void Material::unset_material(tMaterialType flag)
{
    mask = (tMaterialType)(mask & ~(mask & flag));
}

BOOLEAN Material::has_material_type(tMaterialType flag)
{
    return mask & flag ? TRUE : FALSE;
}

void Material::draw(const Matrix4& C)
{
    render();
    Group::draw(C);
    set_default_material();
}

void Material::render()
{
    if (mask & AMBIENT)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.get_pointer());
    }
    if (mask & DIFFUSE)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.get_pointer());
    }
    if (mask & SPECULAR)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.get_pointer());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
    if (mask & EMISSION)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission.get_pointer());
    }
}

void Material::set_default_material()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, default_ambient.get_pointer());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, default_diffuse.get_pointer());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, default_specular.get_pointer());
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, default_emission.get_pointer());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, default_shininess);
}