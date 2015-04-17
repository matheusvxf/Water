#include "Texture.h"
#include "Utility.h"

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <cassert>

static float default_texture_array[] = { 0, 0, 1, 0, 1, 1, 0, 1 };

static unsigned char* loadPPM(const char* filename, int& width, int& height);

Texture::Texture() : Texture(tTextureType::TEXTURE_2D) {}

Texture::Texture(tTextureType type) : texture_type(type)
{
    glGenTextures(1, &texture);
    texture_array = default_texture_array;
}

void Texture::init()
{
    
}

Texture::~Texture()
{
}

inline void Texture::draw_texture_2d(const Matrix4& C)
{
    glEnable(GL_TEXTURE_2D);   // enable texture mapping
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexCoordPointer(2, GL_FLOAT, 0, texture_array);
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Select GL_MODULATE to mix texture with polygon color for shading:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Use bilinear interpolation:
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Group::draw(C);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

void Texture::draw_texture_cube_map(const Matrix4& C)
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glActiveTexture(GL_TEXTURE0);
    Group::draw(C);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

void Texture::draw(const Matrix4& C)
{
    switch (texture_type)
    {
    case GL_TEXTURE_2D:
        draw_texture_2d(C);
        break;
    case GL_TEXTURE_CUBE_MAP:
        draw_texture_cube_map(C);
        break;
    }
    GL_DEBUG("Texture::draw");
}

void Texture::loadTexture(const char *f_name)
{
    return loadTexture(f_name, &texture);
}

// load image file into texture object
void Texture::loadTexture(const char *f_name, GLuint *texture_id)
{
    int twidth, theight;   // texture width/height [pixels]
    unsigned char* tdata;  // texture pixel data

    // Load image file
    tdata = loadPPM((char*)f_name, twidth, theight);
    if (tdata == NULL)
    {
        std::cerr << "Error to load texture";
        return;
    }

    glBindTexture(GL_TEXTURE_2D, *texture_id);

    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

    // Set bi-linear filtering for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GL_DEBUG("Texture::loadTexture - end");
}

bool Texture::load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name)
{
    int x, y;
    unsigned char*  image_data;

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    image_data = loadPPM(file_name, x, y);

    if (!image_data)
    {
        fprintf(stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
    {
        fprintf(
            stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
            );
    }

    // copy image data into 'target' side of cube map
    glTexImage2D(side_target, 0, 3, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    free(image_data);

    GL_DEBUG("Texture::load_cube_map_side - end");

    return true;
}

void Texture::make_cube_map(
    const char* front,
    const char* back,
    const char* top,
    const char* bottom,
    const char* left,
    const char* right)
{
    make_cube_map(front, back, top, bottom, left, right, &texture);
}

void Texture::make_cube_map(
    const char* front,
    const char* back,
    const char* top,
    const char* bottom,
    const char* left,
    const char* right,
    GLuint* tex_cube
    )
{
    // generate a cube-map texture to hold all the sides
    glActiveTexture(GL_TEXTURE0);

    // load each image and copy into a side of the cube-map texture
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
    assert(
        load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
    // format cube map texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GL_DEBUG("Texture::make_cube_map");
}


/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;

    if ((fp = fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);

    // Read width and height:
    do
    {
        retval_fgets = fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width = atoi(buf[1]);
    height = atoi(buf[2]);

    // Read maxval:
    do
    {
        retval_fgets = fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    return rawData;
}