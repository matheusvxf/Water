#include "Cone.h"

#include "GLee.h"

static float default_slices = 64;
static float default_stacks = 64;

Cone::Cone() : Cone(1.0, 1.0) {} 

Cone::Cone(float base, float height) : Cone(base, height, default_slices, default_stacks) {}

Cone::Cone(float base, float height, UINT32 slices, UINT32 stacks) : base(base), height(height), slices(slices), stacks(stacks) {}

Cone::~Cone()
{
}

void Cone::render()
{
    glutSolidCone(base, height, slices, stacks);
}