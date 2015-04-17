#include "Patch.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Config.h"
#include "utility.h"
#include "main.h"

#include <cmath>

static Matrix4 B_bez(
    Vector4(-1, 3, -3, 1),
    Vector4(3, -6, 3, 0),
    Vector4(-3, 3, 0, 0),
    Vector4(1, 0, 0, 0));
static const UINT32 n_u = 16, n_v = 16;
static Matrix4 Gx, Gy, Gz;

static Vector4 bez(float u, const Matrix4& C);
static Vector4 bez_dt(float t, const Matrix4& C);

static const char reflection_vertex_shader[] = { "resources/shader/reflection.vert" };
static const char reflection_frag_shader[] = { "resources/shader/reflection.frag" };

#define STOP_ANIMATION FALSE

inline Matrix4 bez_make_c(const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3)
{
    static Matrix4 G_bez;

    G_bez.setColumn(0, p0);
    G_bez.setColumn(1, p1);
    G_bez.setColumn(2, p2);
    G_bez.setColumn(3, p3);

    return G_bez * B_bez;
}

inline Vector4 bez(float u, const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3)
{
    return bez(u, bez_make_c(p0, p1, p2, p3));
}

inline Vector4 bez(float u, const Matrix4& C)
{
    Vector4 T(u * u * u, u * u, u, 1);

    return C * T;
}

inline Vector4 bez_dt(float t, const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3)
{
    return bez_dt(t, bez_make_c(p0, p1, p2, p3));
}

inline Vector4 bez_dt(float t, const Matrix4& C)
{
    Vector4 T_dt(3 * t * t, 2 * t, 1, 0);

    return C * T_dt;
}

Patch::Patch() : Patch(0) {}

Patch::Patch(float phase) : phase(phase)
{
    reflection = new Shader(reflection_vertex_shader, reflection_frag_shader);
    (*reflection)[0] = glGetUniformLocation(reflection->getPid(), "CameraPosition");
    (*reflection)[1] = glGetUniformLocation(reflection->getPid(), "CameraMatrix");

    GL_DEBUG("Patch::Patch");
    vertice = new float[n_u * n_v * 12];
    normal = new float[n_u * n_v * 12];
    normal_line = new float[n_u * n_v * 12 * 2];
    n_vertice = (n_u * n_v * 12) / 3;

    update();
}

Patch::~Patch()
{
    delete[]vertice;
    delete[]normal;
    delete[]normal_line;
    
}

void Patch::update()
{
    static float t;
    static float frequency = 0.0001;

#if (STOP_ANIMATION == TRUE)
    t += 0;
#else
    t += 10;
#endif
    /* Set points left to right, bottom to top */
    for (UINT8 i = 0; i < 16; ++i)
    {
        /*Gx[i / 4][i % 4] = -1.5 + i % 4;
        Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);
        Gz[i / 4][i % 4] = -1.5 + i / 4;
        if (i % 4 == 0)
        {
            //Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);
        }
        else if (i % 4 == 1)
        {
            Gy[i / 4][i % 4] = 1 + sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i - 1) % 4) + phase);
            //Gx[i / 4][i % 4] = -1.5 + i % 4 - sin(2 * M_PI * frequency * 2 * t) / 2.0;
        }
        else if (i % 4 == 2)
        {
            //Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i + 1) % 4) + phase);
            //Gx[i / 4][i % 4] = -1.5 + i % 4 + sin(2 * M_PI * frequency * 2 * t) / 4.0;
        }
        else if (i % 4 == 3)
        {
            //Gy[i / 4][i % 4] = -sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);
        }
        */
        Gx[i / 4][i % 4] = -1.5 + i % 4;
        Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);
        Gz[i / 4][i % 4] = -1.5 + i / 4;
        if (i % 4 == 0)
        {
            Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);

        }
        else if (i % 4 == 1)
        {
            Gy[i / 4][i % 4] = sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i - 1) % 4) + phase);
            Gx[i / 4][i % 4] = -1.5 + i % 4 - sin(2 * M_PI * frequency * 2 * t) / 4.0;
            Gz[i / 4][i % 4] = -1.5 + i / 4 + 2 * sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i - 1) % 4) + phase);
        }
        else if (i % 4 == 2)
        {
            Gy[i / 4][i % 4] = -sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i + 1) % 4) + phase);
            Gx[i / 4][i % 4] = -1.5 + i % 4 + sin(2 * M_PI * frequency * 2 * t) / 4.0;
            Gz[i / 4][i % 4] = -1.5 + i / 4 - 2 * sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * ((i - 1) % 4) + phase);
        }
        else if (i % 4 == 3)
        {
            Gy[i / 4][i % 4] = -sin(2 * M_PI * frequency * t + (2.0 * M_PI / 3.0) * (i % 4) + phase);
        }
#if(STOP_ANIMATION == TRUE)
        Gy[i / 4][i % 4] = 0;
#endif

        points[i].set(Gx[i / 4][i % 4], Gy[i / 4][i % 4], Gz[i / 4][i % 4], 1);
    }

    generate_patch();
}

void Patch::generate_patch()
{
    static Vector4 x[4], x1_dv, x2_dv, x3_dv, x4_dv, x1_du, x2_du, x3_du, x4_du;
    static Vector3 n[4];
    static Vector4 tmp;
    float u1, u2, v1, v2;
    Vector4 q1[4], q2[4], bez_v1[4], bez_v2[4], q1_du[4], q2_du[4];
    UINT32 index;
    static Matrix4 C_u[4], C_v[4], C_q1, C_q2, C_q1_du, C_q2_du;
    GLfloat vec3[3];
    UINT32 normal_index = 0;

    for (register UINT8 i = 0; i < 4; ++i)
    {
        /* Curves going left to  right */
        C_u[i] = bez_make_c(points[i * 4], points[i * 4 + 1], points[i * 4 + 2], points[i * 4 + 3]);
        /* Curves going bottom up */
        C_v[i] = bez_make_c(points[i], points[i + 4], points[i + 8], points[i + 12]);
    }

    glNormal3b(0, 1, 0);
    glColor3b(1, 1, 1);
    for (UINT32 i = 0; i < n_u; ++i) /* Vary in the vertical */
    {
        u1 = (float)i / (float)n_u;
        u2 = (float)(i + 1) / (float)n_u;

        for (register UINT8 q_n = 0; q_n < 4; ++q_n)
        {
            q1[q_n] = bez(u1, C_u[q_n]); /* Horizontal curve 1 */
            q2[q_n] = bez(u2, C_u[q_n]); /* Horizontal curve 2 */
        }

        C_q1 = bez_make_c(q1[0], q1[1], q1[2], q1[3]);
        C_q2 = bez_make_c(q2[0], q2[1], q2[2], q2[3]);

        for (UINT32 j = 0; j < n_v; ++j) /* Vary in the horizontal */
        {
            v1 = (float)j / (float)n_v;
            v2 = (float)(j + 1) / (float)n_v;

            x[0] = bez(v1, C_q1); /* bottom left */
            x[1] = bez(v2, C_q1); /* bottom right */
            x[2] = bez(v2, C_q2); /* top right */
            x[3] = bez(v1, C_q2); /* top left */

            x1_dv = bez_dt(v1, C_q1);
            x2_dv = bez_dt(v2, C_q1);
            x3_dv = bez_dt(v2, C_q2);
            x4_dv = bez_dt(v1, C_q2);

            for (register UINT8 k = 0; k < 4; ++k)
            {
                bez_v1[k] = bez(v1, C_v[k]);
                bez_v2[k] = bez(v2, C_v[k]);
            }

            x1_du = bez_dt(u1, bez_v1[0], bez_v1[1], bez_v1[2], bez_v1[3]);
            x2_du = bez_dt(u1, bez_v2[0], bez_v2[1], bez_v2[2], bez_v2[3]);
            x3_du = bez_dt(u2, bez_v2[0], bez_v2[1], bez_v2[2], bez_v2[3]);
            x4_du = bez_dt(u2, bez_v1[0], bez_v1[1], bez_v1[2], bez_v1[3]);

            n[0] = x1_dv.cross(x1_du).normalize();
            n[1] = x2_dv.cross(x2_du).normalize();
            n[2] = x3_dv.cross(x3_du).normalize();
            n[3] = x4_dv.cross(x4_du).normalize();

            index = i * n_v * 12 + j * 12;

            for (register UINT8 z = 0; z < 4; ++z)
            {
                for (register UINT8 w = 0; w < 3; ++w)
                {
                    normal[index + z * 3 + w] = n[z][w];
                    vertice[index + z * 3 + w] = x[z][w];
                    normal_line[normal_index] = x[z][w];
                    normal_index++;
                }

                for (register UINT8 w = 0; w < 3; ++w)
                {
                    normal_line[normal_index] = x[z][w] + n[z][w];
                    normal_index++;
                }

            }
        }
    }
}

void Patch::render()
{
    float vec3[3];
    float mat4[16];

#if(DEBUG == TRUE)
    if (Globals::geometry_debug)
    {
        for (register UINT8 i = 0; i < 16; ++i)
        {
            glPushMatrix();
            glTranslated(points[i][0], points[i][1], points[i][2]);
            glutSolidSphere(0.1, 16, 16);
            glPopMatrix();
        }

        glEnableClientState(GL_VERTEX_ARRAY);


        glVertexPointer(3, GL_FLOAT, 0, normal_line);

        glColor3f(0.5, 0.5, 0.5);
        glDrawArrays(GL_LINES, 0, n_vertice * 2);

        glDisableClientState(GL_VERTEX_ARRAY);
    }
#endif

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if (Globals::environment_mapping)
    {
        reflection->bind();
        Globals::camera->center_of_proj().get_pointerf(vec3);
        glUniform3fv((*reflection)[0], 1, vec3);
        Globals::camera->getMatrix().get_pointerf(mat4);
        glUniformMatrix4fv((*reflection)[1], 1, GL_TRUE, mat4);
    }
    else
    {
        reflection->unbind();
    }

    glVertexPointer(3, GL_FLOAT, 0, vertice);
    glNormalPointer(GL_FLOAT, 0, normal);

    glDrawArrays(GL_QUADS, 0, n_vertice);

    reflection->unbind();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    GL_DEBUG("Patch::render - end");
}
