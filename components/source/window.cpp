#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "main.h"
#include "Config.h"
#include "Utility.h"

#include <iostream>

using namespace std;

int Window::width = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
    Globals::world->update();

    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    cerr << "Window::reshapeCallback called" << endl;
    width = w;
    height = h;
    Globals::aspect = (double)width / (double)height;
    glViewport(0, 0, w, h);  // set new view port size
    setProjection();
    World::calculate_frustum();
}

void Window::setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(Globals::FOV, double(width) / (double)height, Globals::near_plane, Globals::far_plane); // set perspective projection viewing frustum
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers

    Globals::world->draw(Matrix4::matrix_identity);
#if (DEBUG_GRAPHICS == TRUE) /* Draw the axis x, y, z */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    draw_line(-1000, 0, 0, 1000, 0, 0);
    draw_line(0, -1000, 0, 0, 1000, 0);
    draw_line(0, 0, -1000, 0, 0, 1000);
#endif

    glFlush();
    glutSwapBuffers();
}
