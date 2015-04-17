#include "Skybox.h"
#include "MatrixTransform.h"
#include "Texture.h"
#include "Utility.h"
#include "Shader.h"
#include "Box.h"

static const char front[] = { "resources/front.ppm" };
static const char back[] = { "resources/back.ppm" };
static const char left[] = { "resources/left.ppm" };
static const char right[] = { "resources/right.ppm" };
static const char top[] = { "resources/top.ppm" };
static const char bottom[] = { "resources/bottom.ppm" };



Skybox::Skybox()
{
    MatrixTransform *transform;
    Texture *texture;
    Matrix4 tmp;

    tmp.makeScale(1.5);
    tmp *= Matrix4(Matrix4::tTranslate, 0, 0, 0);
    transform = new MatrixTransform(tmp.clone());
    texture = new Texture(Texture::tTextureType::TEXTURE_CUBE_MAP);
    texture->make_cube_map(back, front, top, bottom, left, right);
    transform->add_child(new Box());
    texture->add_child(transform);
    this->add_child(texture);
}

Skybox::~Skybox()
{
}

void Skybox::draw(const Matrix4& C)
{
    glDepthMask(GL_FALSE);
    Group::draw(C);
    glDepthMask(GL_TRUE);

    GL_DEBUG("Skybox::draw");
}
