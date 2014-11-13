#include <iostream>
#include <chrono>

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
#include "node.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
double Window::fov = 60.0;  // perspective frustum vertical field of view in degrees
double Window::leftBound = 0;
double Window::rightBound = 0;
double Window::topBound = 0;
double Window::bottomBound = 0;

namespace Scene
{
	Camera *camera = nullptr;
	Group *world = nullptr;
	vector<Node*> nodeList;
	vector<Robot*> robotList;
	bool showBounds = false;
	bool frustumCulling = false;

	// Create a new robot at the given position in world coordinates
	Robot* createRobot(Vector3& pos) {
		double x, y, z;
		x = pos.getX();
		y = pos.getY();
		z = pos.getZ();
		Robot *roboPtr = new Robot(x, y, z);
		robotList.push_back(roboPtr);
		return roboPtr;
	};

	// Initialize pointers with defaults
	void setup() {
		camera = new Camera(
			Vector3(0, 10, 50), Vector3(0, 0, 0), Vector3(0, 1, 0)
		);
		world = new Group();

		double robotSpacing = 10;
		double platoonWidth = 100;
		for (double x = -platoonWidth; x < platoonWidth; x += robotSpacing) {
			for (double y = -platoonWidth; y < platoonWidth; y += robotSpacing) {
		//		world->addChild(createRobot(Vector3(x, 0, y)));
			}
		}

		//    \/ Debug Robot
		Robot *ptr = createRobot(Vector3(0, 0, 10));
		world->addChild( ptr );
	};
	void dealloc() {
		for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
			delete *iter;
			*iter = nullptr;
		}
		delete world; world = nullptr;
	};
};


//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
    // Call draw on the Scene
	displayCallback(); // call display routine to show the cube
	
	// Geometry was drawn, then moved for animations, so recalculate culling bounds
	// AFTER calling draw
	Scene::world->updateBounds();

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
  glMatrixMode(GL_MODELVIEW);

  // Recalculate the bounds
  double fovRadians = (Window::fov / 2) / 180.0 * M_PI;
  double aspectRatio = ((double)(width))/(double)height;

  Window::bottomBound = -20 * tan( fovRadians );
  Window::topBound = -Window::bottomBound;

  Window::leftBound = Window::bottomBound * aspectRatio;
  Window::rightBound = Window::topBound * aspectRatio;

  /* cerr << "Bottom: " << Window::bottomBound 
       << " | Top: " << Window::topBound << endl
       << "Left: " << Window::leftBound 
       << " | Right: " << Window::rightBound << endl;*/ // Left over from PA 1

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{

  // Start the timer for this frame
  auto c_start = chrono::high_resolution_clock::now();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

  // Draw our scene so long as it is actually in memory
  if ( Scene::camera && Scene::world ) {

	// Pass in our inverse camera matrix in row-major order (since our draw function
	// in our Node classes expects row-major matrices)
	Scene::world->draw(Scene::camera->getInverseMatrix());

  }

  glFlush();  
  glutSwapBuffers();

  // Stop the timer for this frame
  auto c_end = chrono::high_resolution_clock::now();
  cerr << "FPS: " << ( 1.0 / chrono::duration<double, milli>(c_end - c_start).count()) << endl;

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
  case 'b':
	  Scene::showBounds = !Scene::showBounds;
	  Scene::world->showBoundingBox(Scene::showBounds);
	  cerr << "Bounding spheres are " << (Scene::showBounds ? "on" : "off") << endl;
	  break;
  case 'c':
	  Scene::frustumCulling = !Scene::frustumCulling;
	  cerr << "Culling is " << (Scene::frustumCulling ? "on" : "off" ) << endl;
	  break;
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