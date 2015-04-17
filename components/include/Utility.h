#pragma once

#include "Config.h"
#include "World.h"
#include "Vector4.h"

#include <cmath>
#include "GL/glut.h"


#define M_PI 3.14159265358979323846

#if (UTILITY_MACRO == TRUE)

#define degree2rad(angle) (angle / 180.0 * M_PI)
#define rad2degree(rad) (180.0 * rad / M_PI )
#define foreach(v, it) for(decltype(v.begin()) it = v.begin(); it != v.end(); ++it)
#define rand_int(min, max) (rand() % (max - min) + min)
#define min_fov(height, distance) rad2degree(atan2(height, distance));
#define draw_line(x1, y1, z1, x2, y2, z2) \
    glBegin(GL_LINES); \
    glVertex3f((x1), (y1), (z1)); \
    glVertex3f((x2), (y2), (z2)); \
    glEnd();
/* a = b - c */
#define vector(a,b,c) \
    (a)[0] = (b)[0] - (c)[0];	\
    (a)[1] = (b)[1] - (c)[1];	\
    (a)[2] = (b)[2] - (c)[2]
#define crossProduct(a,b,c) \
    (a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
    (a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
    (a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];
#define innerProduct(v,q) \
    ((v)[0] * (q)[0] + \
    (v)[1] * (q)[1] + \
    (v)[2] * (q)[2])
#define LENGTH(a) \
    (sqrtf(innerProduct(a, a)))
#define VEC_DISTANCE(a, b) \
    vector(a, a, b); \
    LENGTH(a)

#else

/*******************************************************************************
*
*	Function		degree2rad
*		
*	Convert from degrees to radians
*
*	angle[in]:
*
*	Return	the angle in radians
*
*******************************************************************************/
double degree2rad(double angle);

#endif /* UTILITY_MACRO */

#if (DEBUG == TRUE)
#define GL_DEBUG(msg) gl_debug(__FILE__, __LINE__, msg)
#else
#define GL_DEBUG(msg) {}
#endif

extern bool load_texture(char *TexName, GLuint TexHandle);
extern void set_shaders();
extern double distance2plane(const Plane& plane, const Vector4& point);
extern void drawString(void *font, char *s, float x, float y, float z);
extern int rayIntersectsTriangle(float *p, float *d, float *v0, float *v1, float *v2);
extern char *gl_error2str(GLenum error);
extern void gl_debug(const char *file, UINT32 line, char *msg = "\0");


