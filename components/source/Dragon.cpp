#include "Dragon.h"
#include "Config.h"

#include <string>

static std::string file = "Resources/dragon.obj";

#if (COMPILE_MODEL == TRUE)
#include "Dragon_model.h"

Dragon::Dragon() : ObjectModel(dragon_vertice, dragon_n_vertice, dragon_normal, dragon_n_normal, dragon_face, dragon_n_face)
{
    material.set_property(Material::AMBIENT, Color(0.1, 0.2, 0.1));
    material.set_property(Material::DIFFUSE, Color(0.9, 0.9, 0.1));
    material.set_property(Material::SPECULAR, Color(0.4, 0.1, 0.4));
    material.set_property(Material::SHININESS, 10);
}
#else
Dragon::Dragon() : ObjectModel() {}
#endif

Dragon::~Dragon() {}