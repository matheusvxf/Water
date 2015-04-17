#pragma once

#include "DataType.h"
#include "Group.h"

class Texture :
    public Group
{
public:
    enum tTextureType
    {
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
    };
private:
    void draw_texture_2d(const Matrix4& C);
    void draw_texture_cube_map(const Matrix4& C);
protected:
    GLuint texture;
    tTextureType texture_type;
    float *texture_array;
public:
    Texture();
    Texture(tTextureType type);
    ~Texture();

    virtual void draw(const Matrix4& C);

    void loadTexture(const char *f_name);
    void make_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right);
    static void make_cube_map(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, GLuint* tex_cube);
    static void init();
    static void loadTexture(const char *f_name, GLuint *texture_id);
    static bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);
};

