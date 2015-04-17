#include "tga.h"
#include "Utility.h"

#include <iostream>

#if (UTILITY_MACRO == FALSE)

double degree2rad(double angle){
	return angle / 180 * M_PI;
}

#endif

bool load_texture(char *TexName, GLuint TexHandle)
{
    TGAImg Img;        // Image loader

    // Load our Texture
    if (Img.Load(TexName) != IMG_OK)
        return false;

    glBindTexture(GL_TEXTURE_2D, TexHandle); // Set our Tex handle as current

    // Create the texture
    if (Img.GetBPP() == 24)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
    else if (Img.GetBPP() == 32)
        glTexImage2D(GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg());
    else
        return false;

    // Specify filtering and edge actions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return true;
}

double distance2plane(const Plane& plane, const Vector4& point)
{
    return (point - plane.center).dot(plane.normal);
}

void drawString(void *font, char *s, float x, float y, float z)
{
    unsigned int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter(font, s[i]);
}

int rayIntersectsTriangle(float *p, float *d, float *v0, float *v1, float *v2)
{
    float e1[3], e2[3], h[3], s[3], q[3];
    float a, f, t, u, v;
    vector(e1, v1, v0);
    vector(e2, v2, v0);

    crossProduct(h, d, e2);
    a = innerProduct(e1, h);

    if (a > -0.00001 && a < 0.00001)
        return(false);

    f = 1 / a;
    vector(s, p, v0);
    u = f * (innerProduct(s, h));

    if (u < 0.0 || u > 1.0)
        return(false);

    crossProduct(q, s, e1);
    v = f * innerProduct(d, q);

    if (v < 0.0 || u + v > 1.0)
        return(false);

    // at this stage we can compute t to find out where
    // the intersection point is on the line
    t = f * innerProduct(e2, q);

    if (t > 0.00001) // ray intersection
        return(true);

    else // this means that there is a line intersection
        // but not a ray intersection
        return (false);
}

char *gl_error2str(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    default: return "unkown";
    }
}

void gl_debug(const char *file, UINT32 line, char *msg)
{
    GLenum error;

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << gl_error2str(error) << "(" << error << ") at " << file << ": " << line << " - " << msg << std::endl;
    }
}