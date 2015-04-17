#include "Bear.h"
#include "Config.h"

#include <string>

static std::string file = "Resources/bear.obj";

#if (COMPILE_MODEL == TRUE)
#include "Bear_model.h"

Bear::Bear() : ObjectModel(bear_vertice, bear_n_vertice, bear_normal, bear_n_normal, bear_face, bear_n_face)
{
    material.set_property(Material::SPECULAR, Color(0.9, 0.9, 0.9));
    material.set_property(Material::SHININESS, 150);
    material.set_property(Material::DIFFUSE, Color(0.8, 0.8, 0.8));
}
#else
Bear::Bear() : ObjectModel() {}
#endif

Bear::~Bear() {}