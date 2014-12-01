#include <iostream>
#include "shader.h"

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include <math.h>
#include "main.h"
#include "Window.h"
#include "SpotLight.h"
#include "BezierPatch.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
double Window::fov = 60.0;  // perspective frustum vertical field of view in degrees
double Window::ROTSCALE = 0.125; // tutorial says to use 90.0
double Window::ZOOMSCALE = 0.008;
int Window::mouseStartX = 0;
int Window::mouseStartY = 0;
bool Window::rotating = false;
bool Window::zooming = false;
Vector3 Window::lastPoint = Vector3();;
Matrix4 Window::preScrollMtx = Matrix4::translate(1,1,1); // Identity
Matrix4 Window::tmpMatrix = Matrix4::translate(1, 1, 1);

namespace Scene
{
	Camera *camera = nullptr;
	MatrixTransform *world = nullptr; // Top level of the scene graph
	ObjModel *bunny, *dragon, *bear = nullptr;
	PointLight *ptLight;
	SpotLight *spotLight;
	vector<Node*> nodeList;
	vector<Robot*> robotList;
	vector<Plane> frustumList = vector<Plane>(6); // Culling doesn't work
	Shader *shader;
	BezierPatch *waterPatch;
	MatrixTransform *patchScale, *patchTranslate;
	bool showBounds = false;
	bool frustumCulling = false;
	bool showFps = false;
	bool shaderOn = false;
	double znear = 1.0;
	double zfar = 1000; //1000.0;

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
			Vector3(0, 0, -20), Vector3(0, 0, 0), Vector3(0, 1, 0)
		);

		world = new MatrixTransform();
		bunny = new ObjModel();
		dragon = new ObjModel();
		bear = new ObjModel();

		double fovRadians = (Window::fov / 2) / 180.0 * M_PI;
		double aspectRatio = ((double)(Window::width)) / (double)Window::height;
		double windowWidth = 2 * tan(fovRadians) * aspectRatio * Scene::camera->getPos().getZ();

		shader = new Shader("diffuse_shading.vert", "diffuse_shading.frag", true);
		shader->printLog("LOADING SHADER: ");

		Vector4 p0, p1, p2, p3;
		p0 = Vector4(-5,0,0,1);
		p1 = Vector4(-2.5,-7,0,1);
		p2 = Vector4(2.5,7,0,1);
		p3 = Vector4(5,0,0,1);
		waterPatch = new BezierPatch();
		Matrix4 scl = Matrix4::scale(50,50,50);
		Matrix4 trn = Matrix4::translate(0.0,-1.0,0.0);
		patchScale = new MatrixTransform( scl );
		patchTranslate = new MatrixTransform( trn );

		// Define the lighting and nodes in the scene

		spotLight = new SpotLight(0, -4, -10, 0.0);
		spotLight->setCutoff(20.0);
		spotLight->setSpecular(0, 1, 1, 1); // blue as fuck
		spotLight->setDiffuse(0, 0, 1, 1);
		spotLight->setAmbient(0, 0, 1, 0);
		spotLight->setSpotExponent(0);
		spotLight->setSpotDir(Vector3(0, 0, -1));

		ptLight = new PointLight(0, 2, -10);
		ptLight->setAmbient(0, 0.25, 0, 1);
		ptLight->setSpecular(0, 1, 0, 1);
		ptLight->setDiffuse(0, 1, 0, 0); // green 

		world->addChild( patchTranslate ); 
		patchTranslate->addChild( patchScale );
		patchScale->addChild( waterPatch );

	};
	// Deallocate all kinds of stuff
	void dealloc() {
		for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
			delete *iter;
			*iter = nullptr;
		}
		delete world; world = nullptr;
		delete bunny, dragon, bear;
		delete ptLight; delete spotLight;
		delete waterPatch;
		delete patchScale, patchTranslate;
		waterPatch = nullptr; patchScale = patchTranslate = nullptr;
		bunny = dragon = bear = nullptr;
		ptLight = nullptr; spotLight = nullptr;
	};
};


//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{

	static int frame = 0, time, timebase = 0;

	Scene::world->setCulling(Scene::frustumCulling);

    // Call draw on the Scene
	displayCallback(); // call display routine to show the cube

	/* FPS Counter courtesy of Lighthouse3D */

	frame++; // Increment the number of frames we've drawn
	time = glutGet(GLUT_ELAPSED_TIME);

	// Reset the counter and print the FPS if our timer has lasted longer than a 
	// second.
	if (time - timebase > 1000 && Scene::showFps) {
		cerr << "FPS: " << frame * 1000 / (time - timebase) << endl;
		timebase = time; // Set timebase to the current time
		frame = 0; // Reset frame counter
	}

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
                  Scene::znear, 
                  Scene::zfar);  // set perspective projection viewing frustum
  glMatrixMode(GL_MODELVIEW);

  // Update Window::...
  Window::width = w;
  Window::height = h;

  // Do some important calculations

  double fovRadians = (Window::fov / 2) / 180.0 * M_PI;
  double aspectRatio = ((double)(width))/(double)height;
  double windowWidth = 2 * tan(fovRadians) * aspectRatio * Scene::camera->getPos().getZ();

  // Calculate the height and width of the near and far clipping planes

  double hNear = 2 * tan( fovRadians ) * Scene::znear;
  double wNear = hNear * aspectRatio;
  double hFar = 2 * tan(fovRadians) * Scene::zfar;
  double wFar = hNear * aspectRatio;

  // Erase everything in the frustum plane list for our Scene and recalculate

  unsigned int originalSize = Scene::frustumList.size();
  for (unsigned int i = 0; i < originalSize; i++) {
	  Scene::frustumList.pop_back();
  }

  // Get camera axes
  Vector3 d = Scene::camera->getLookDir();
  Vector3 up = Scene::camera->getUp();
  Vector3 right = Scene::camera->getRight();
  Vector3 cam = Scene::camera->getPos();
  d.normalize();
  up.normalize();
  right.normalize();

  d.print("Camera look direction: ");
  up.print("Camera up direction: ");
  right.print("Camera right direction: "); // these look correct

  Vector3 farCenter, nearCenter, nearNormal;
  farCenter = cam + (d * Scene::zfar);
  nearCenter = cam + (d * Scene::znear);
  nearNormal = Scene::camera->getLookDir();

  Plane nearPlane, farPlane, leftPlane, rightPlane, topPlane, bottomPlane;

  // Remember, our normals should point ***OUTSIDE*** of the view frustum

  nearPlane = Plane(nearNormal, nearCenter); 
  nearNormal.negate();
  farPlane = Plane(nearNormal, farCenter);
  
  // Get the point tmp on the right edge of the near plane
  // and subtract it from the camera's position so we get a vector
  // that points in the direction parallel to the right plane

  Vector3 edgePoint = (nearCenter + right * (wNear / 2.0));
  Vector3 tmp = edgePoint - cam;
  tmp.normalize();
  tmp = tmp.cross( tmp, up );

  rightPlane = Plane(tmp, edgePoint);

  // rightPlane is straight from the formula

  // Left Plane

  edgePoint = (nearCenter + right * (-wNear / 2.0));
  tmp = edgePoint - cam; 
  tmp.normalize();
  tmp.cross(up, tmp);

  leftPlane = Plane(tmp, edgePoint);

  // Top Plane

  edgePoint = (nearCenter + up * (hNear / 2.0));
  tmp = edgePoint - cam;
  tmp.normalize();
  tmp.cross(right, tmp);

  topPlane = Plane(tmp, edgePoint);

  // Bottom Plane

  edgePoint = (nearCenter + up * (-hNear / 2.0));
  tmp = edgePoint - cam;
  tmp.normalize();
  tmp.negate();
  tmp.cross(right, tmp);

  bottomPlane = Plane(tmp, edgePoint);

  // Once we've calculated everything, add our planes to the vector

  Scene::frustumList.push_back(nearPlane); // wrong?
  Scene::frustumList.push_back(farPlane); // correct
  Scene::frustumList.push_back(leftPlane); // correct (when we flip the T/F return in Plane, the army disappears)
  Scene::frustumList.push_back(rightPlane); // correct
  Scene::frustumList.push_back(topPlane); // correct
  Scene::frustumList.push_back(bottomPlane); // correct

  // Tell the world what's up
  Scene::world->setVector(&Scene::frustumList);

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{

  if (glGetError() != GL_NO_ERROR) cerr << "GL Error" << endl;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
  
  Matrix4 ident = Matrix4();
  Matrix4 invCam = Matrix4();
  invCam.identity();
  ident.identity();
  if ( Scene::camera != nullptr) 
  	invCam = Scene::camera->getInverseMatrix();

  // Draw our scene so long as it is actually in memory
  if ( Scene::camera && Scene::world ) {

	if (Scene::shaderOn) {
		Scene::shader->bind();
	}
	else {
		Scene::shader->unbind();
	}

	//Scene::waterPatch->draw(100);
	Scene::ptLight->draw(ident);
	//Scene::spotLight->draw(ident);
	Scene::world->draw(invCam);
	
  }

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
  case 'b':
	  Scene::showBounds = !Scene::showBounds;
	  Scene::world->showBoundingBox(Scene::showBounds);
	  cerr << "Bounding spheres are " << (Scene::showBounds ? "on" : "off") << endl;
	  break;
  case 'c':
	  Scene::frustumCulling = !Scene::frustumCulling;
	  Scene::world->setCulling(Scene::frustumCulling);
	  cerr << "Culling is " << (Scene::frustumCulling == true ? "on" : "off" ) << endl;
	  break;
  case 'p':
	  Scene::shaderOn = !Scene::shaderOn;
	  // ** TODO ** enable/disable the shader
	  cerr << "Shaders are " << (Scene::shaderOn == true ? "on" : "off") << endl;
	  break;
  case 'f':
	  Scene::showFps = !Scene::showFps;
	  cerr << "FPS counter is " << (Scene::showFps ? "on" : "off") << endl;
	  break;
  case 'x':
	  transformation = Matrix4::translate(-1.0, 0.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'X':
	  transformation = Matrix4::translate(1.0, 0.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'y':
	  transformation = Matrix4::translate(0.0, -1.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'Y':
	  transformation = Matrix4::translate(0.0, +1.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'z':
	  transformation = Matrix4::scale(0.9, 0.9, 0.9);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'Z':
	  transformation = Matrix4::scale(1.1, 1.1, 1.1);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'o':
	  transformation = Matrix4::rotY(-1.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'O':
	  transformation = Matrix4::rotY(+1.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'r':
	  Scene::world->getMatrix().identity();
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
  case GLUT_KEY_F1: // Load Bunny
	  cout << "Pressed F1" << endl;
	  break;
  default:
	  cout << "Pressed a function key or trigged glutSpecialFunc" << endl;
	  break;
  }
};