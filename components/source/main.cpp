#include "main.h"
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Matrix4.h"
#include "Utility.h"
#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define SHADER 4

#if (SHADER == 1) /* simple */
const static char vertex_shader_f[] = "resources/shader/simple.vert";
const static char frag_shader_f[] = "resources/shader/simple.frag";
#elif (SHADER == 2)
const static char vertex_shader_f[] = "resources/shader/diffuse_shading.vert";
const static char frag_shader_f[] = "resources/shader/diffuse_shading.frag";
#elif (SHADER == 3)
const static char vertex_shader_f[] = "resources/shader/texture2D.vert";
const static char frag_shader_f[] = "resources/shader/texture2D.frag";
#else
const static char vertex_shader_f[] = "resources/shader/shader.vert";
const static char frag_shader_f[] = "resources/shader/shader.frag";
#endif

namespace Globals
{
    Camera* camera;
    Shader* shader;
    World* world;
    Object** curr_obj;
    BOOLEAN draw_bounding_sphere = FALSE;
    BOOLEAN control_light = FALSE;
    BOOLEAN per_pixel_lighting = FALSE;
    BOOLEAN culling = TRUE;
    BOOLEAN geometry_debug = TRUE;
    BOOLEAN environment_mapping = TRUE;
    double FOV = 60.0;
    double near_plane = 1.0;
    double far_plane = 1000.0;
    double aspect = 1.0;
    tScene scene = tBunny;
    GLuint vertexShaderObject, fragmentShaderObject;
};

// initialize OpenGL state
void initGL()
{
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float shininess[] = { 100.0 };
    float position[] = { 0.0, 10.0, 1.0, 0.0 };	// light source position

    glShadeModel(GL_SMOOTH);   // enable smooth shading
    glClear(GL_DEPTH_BUFFER_BIT);   // clear depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // black background
    glClearDepth(1.0f);        // depth buffer setup
    glEnable(GL_DEPTH_TEST);   // enables depth testing
    glDepthFunc(GL_LEQUAL);    // configure depth testing@
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // really nice perspective calculations

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
    glShadeModel(GL_SMOOTH);             	      // set shading to smooth

    // Generate material properties:
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // Generate light source:
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);   // enable texture mapping
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);      /* initialize GLUT */
    GLeeInit();                 /* Initialize GLEE */

    initGL();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
    glutInitWindowSize(Window::width, Window::height);      // set initial window size
    glutCreateWindow("OpenGL Water");    	      // open window and set window title

    GL_DEBUG("setup");

    // Install callback functions:
    glutDisplayFunc(Window::displayCallback);
    glutReshapeFunc(Window::reshapeCallback);
    glutIdleFunc(Window::idleCallback);
    glutKeyboardFunc(Keyboard::keyPressCb);
    glutMouseFunc(Mouse::mouseCallback);
    glutSpecialFunc(Keyboard::specialFuncCb);

    // Allocate all textures in one go

    /* GLSL */
    if (GLEE_ARB_vertex_shader && GLEE_ARB_fragment_shader)
        printf("Ready for GLSL\n");
    else
    {
        printf("Not totally ready :( \n");
        exit(1);
    }
    
    Texture::init();

    Globals::camera = new Camera();
    Globals::world = new World();
    Globals::shader = new Shader(vertex_shader_f, frag_shader_f);

    GL_DEBUG("initialized camera and world");

    glutMainLoop();
    return 0;
}