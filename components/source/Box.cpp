#include "Box.h"
#include "main.h"

static float default_points[] = {
    -10.0f, 10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    -10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, 10.0f
};

static const char skybox_vertex_shader[] = { "resources/shader/skybox.vert" };
static const char skybox_frag_shader[] = { "resources/shader/skybox.frag" };

Box::Box()
{
    float *points = new float[sizeof(default_points)];
    memcpy(points, default_points, sizeof(default_points)* sizeof(float));
    shader = new Shader(skybox_vertex_shader, skybox_frag_shader);
    (*shader)[0] = glGetUniformLocation(shader->getPid(), "CameraPosition");
    
    init(points);
}

Box::Box(float *points)
{
    init(points);
}

void Box::init(float *points)
{
    float SkyBoxVertices[] =
    {    // x, y, z, x, y, z, x, y, z, x, y, z
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, // +X
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // -X
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // +Y
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // -Y
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // +Z
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f  // -Z
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 288, SkyBoxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Box::~Box()
{
    delete[]points;
}

void Box::render()
{
    float vec3[3];

    shader->bind();

    Globals::camera->center_of_proj().get_pointerf(vec3);
    glUniform3fv((*shader)[0], 1, vec3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 12, (void*)0);
    glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    shader->unbind();
}