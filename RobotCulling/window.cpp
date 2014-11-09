#include <iostream>

#ifndef __APPLE__
#include <GL/glut.h>
#define BALL_DEFAULT_VELOCITY_X .0047
#define BALL_DEFAULT_VELOCITY_Y -.0068
#else
#include <GLUT/glut.h>
#define BALL_DEFAULT_VELOCITY_X .47
#define BALL_DEFAULT_VELOCITY_Y -.68
#endif

#include <math.h>
#include "main.h"
#include "Window.h"
#include "Geode.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
int Window::spinDirection = 1; // 1 is counterclockwise around Y axis
bool Window::ball = false;  // initially, show the cube model
double Window::fov = 60.0;  // perspective frustum vertical field of view in degrees
double Window::leftBound = 0;
double Window::rightBound = 0;
double Window::topBound = 0;
double Window::bottomBound = 0;

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
  displayCallback();         // call display routine to show the cube
};

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
  cerr << "Window::reshapeCallback called" << endl;
  width = w;
  height = h;
  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( Window::fov, 
                  double(width)/(double)height, 
                  1.0, 
                  1000.0);  // set perspective projection viewing frustum
  glTranslatef(0, 0, -20);  // move camera back 20 units so that it looks at the origin (or else it's in the origin)
  glMatrixMode(GL_MODELVIEW);

  // Recalculate the bounds
  double fovRadians = (Window::fov / 2) / 180.0 * M_PI;
  double aspectRatio = ((double)(width))/(double)height;

  Window::bottomBound = -20 * tan( fovRadians );
  Window::topBound = -Window::bottomBound;

  Window::leftBound = Window::bottomBound * aspectRatio;
  Window::rightBound = Window::topBound * aspectRatio;

  cerr << "Bottom: " << Window::bottomBound 
       << " | Top: " << Window::topBound << endl
       << "Left: " << Window::leftBound 
       << " | Right: " << Window::rightBound << endl;

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

  // Draw our scene
  
  glFlush();  
  glutSwapBuffers();
};

//----------------------------------------------------------------------------
// Callback method called by GLUT when keys are pressed
//    char * key     - the key pressed
//    int x          - mouse x position
//    int y          - mouse y position
//    
//    https://www.opengl.org/resources/libraries/glut/spec3/node49.html
//    See main.cpp line 56
void Window::keyboardCallback(unsigned char key, int x, int y) {

  Matrix4 transformation;
  transformation.identity(); // Make sure the Matrix isn't utter garbage

  switch (key) {
    default:
      cerr << "Pressed: " << key << endl;
      break;
  }

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when keys are pressed
//    int key        - the key pressed
//    int x          - mouse x position
//    int y          - mouse y position
//    
//    https://www.opengl.org/resources/libraries/glut/spec3/node54.html
//    See main.cpp line 56
void Window::functionKeysCallback(int key, int x, int y) {
  switch (key) {
  default:
	  cerr << "Pressed a function key or trigged glutSpecialFunc" << endl;
	  break;
  }
};