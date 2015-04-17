#include "Bunny.h"
#include "Config.h"
#include "Material.h"

#include <string>

static std::string file = "Resources/bunny.obj";

#if (COMPILE_MODEL == TRUE)
#include "Bunny_model.h"

Bunny::Bunny() : 
    ObjectModel(bunny_vertice, bunny_n_vertice, bunny_normal, bunny_n_normal, bunny_face, bunny_n_face)
{
    material.set_property(Material::AMBIENT, Color(0.2, 0.2, 0.2));
    material.set_property(Material::DIFFUSE, Color(0.4, 0.4, 0.4));
    material.set_property(Material::SPECULAR, Color(0.4, 0.9, 0.4));
    material.set_property(Material::SHININESS, 100);
}
#else
Bunny::Bunny() : ObjectModel(file) {}
#endif

Bunny::~Bunny() {}