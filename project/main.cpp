#include <iostream>

#include "main.h"
#include "Window.h"

#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include "glee.h"
#include <GL/glut.h>    // Windows and Linux are normal
#endif

using namespace std;

int main(int argc, char *argv[])
{
  float specular[]  = {1.0, 1.0, 1.0, 1.0};
  float shininess[] = {100.0};
  float position[]  = {0.0, 10.0, 0.0, 0.0};	// lightsource position

    
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("A Sylvan Setting");    	      // open window and set window title
  glutFullScreen();

  // Initialize the scene
  Scene::setup();

  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  glEnable(GL_CULL_FACE);                     // enable culling, finally!
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION); 
  
  // Make individual point models look nice
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_NORMALIZE);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  // Generate material properties:
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  
  // Generate light source:
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  glutIdleFunc(Window::idleCallback);
  glutKeyboardFunc(Window::keyboardCallback);
  glutSpecialFunc(Window::functionKeysCallback);
    
  // Start the main loop
  glutMainLoop();

  // If we exit the main loop, dealloc
  Scene::dealloc();

  return 0;
}

