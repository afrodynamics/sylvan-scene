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
#include "SkyBox.h"
#include "BezierPatch.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
double Window::deltaTime = 0;  // milliseconds elapsed between frames
double Window::fov = 60.0;  // perspective frustum vertical field of view in degrees

namespace Scene
{
	Camera *camera = nullptr;
	MatrixTransform *world = nullptr; // Top level of the scene graph
	ObjModel *bunny, *dragon, *bear = nullptr;
	PointLight *ptLight;
	SkyBox *sky;
	vector<Node*> nodeList;
	vector<Robot*> robotList;
	vector<Plane> frustumList = vector<Plane>(6); // Culling doesn't work
	Shader *shader;
	BezierPatch *waterPatch;
	MatrixTransform *patchScale, *skyBoxScale, *patchTranslate;
	bool showBounds = false;
	bool frustumCulling = false;
	bool showFps = false;
	bool shaderOn = false;
	double znear = 1.0;
	double zfar = 1000; //1000.0;
	GLuint textures[6];
	GLuint sky_left, sky_right, sky_up, sky_down, sky_front, sky_back;

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
			Vector3(0, 0, 20), Vector3(0, 0, 0), Vector3(0, 1, 0)
		);

		world = new MatrixTransform();

		double fovRadians = (Window::fov / 2) / 180.0 * M_PI;
		double aspectRatio = ((double)(Window::width)) / (double)Window::height;
		double windowWidth = 2 * tan(fovRadians) * aspectRatio * Scene::camera->getPos().getZ();

		shader = new Shader("reflection_map.vert", "reflection_map.frag", true);
		shader->printLog("LOADING SHADER: ");

		Vector4 p0, p1, p2, p3;
		p0 = Vector4(-5,0,0,1);
		p1 = Vector4(-2.5,-7,0,1);
		p2 = Vector4(2.5,7,0,1);
		p3 = Vector4(5,0,0,1);
		waterPatch = new BezierPatch();
		Matrix4 scl = Matrix4::scale(50,50,50);
		Matrix4 trn = Matrix4::translate(0.0,-10.0,0.0);
		patchScale = new MatrixTransform( scl );
		skyBoxScale = new MatrixTransform( scl );
		patchTranslate = new MatrixTransform( trn );

		// Load a bind the textures
		
		sky = new SkyBox();

		glGenTextures(6, textures);

		sky->right = Window::loadPPM("tex/right.ppm",1024,1024,0);		
		sky->left = Window::loadPPM("tex/left.ppm",1024,1024,1);
		sky->front = Window::loadPPM("tex/front.ppm",1024,1024,2);
		sky->back = Window::loadPPM("tex/back.ppm",1024,1024,3);
		sky->top = Window::loadPPM("tex/top.ppm",1024,1024,4);
		sky->base = Window::loadPPM("tex/base.ppm",1024,1024,5);

		ptLight = new PointLight(0, 2, 0);
		ptLight->setAmbient(0.25, 0.25, 0.25, 1);
		ptLight->setSpecular(0, 0, 1, 1);
		ptLight->setDiffuse(.35, .35, .35, 0); // green

		world->addChild( ptLight );
		world->addChild( patchTranslate ); 
		patchTranslate->addChild( patchScale );
		patchScale->addChild( waterPatch );

		// Sky box needs to be in a separate scene graph so we can bind differeny shaders
		skyBoxScale->addChild( sky );

	};
	// Deallocate all kinds of stuff
	void dealloc() {
		for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
			delete *iter;
			*iter = nullptr;
		}
		delete world; world = nullptr;
		delete bunny, dragon, bear;
		delete ptLight;
		delete waterPatch;
		delete patchScale, patchTranslate, skyBoxScale;
		delete sky;
		sky = nullptr;
		waterPatch = nullptr; patchScale = patchTranslate = nullptr;
		bunny = dragon = bear = nullptr;
		ptLight = nullptr;
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
	if (time - timebase > 1000) {
		// Always calculate delta time
		deltaTime = (time - timebase) - 1000;
		if ( Scene::showFps ) 
			cerr << "FPS: " << frame * 1000 / (time - timebase) << " | DT " << deltaTime << endl;
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

};

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{

  long err = glGetError();
  if (err != GL_NO_ERROR) cerr << gluErrorString( err ) << endl;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
  
  Matrix4 ident = Matrix4();
  Matrix4 invCam = Matrix4();
  invCam.identity();
  ident.identity();
  if ( Scene::camera != nullptr) 
  	invCam = Scene::camera->getGLMatrix();

  // Used by the skybox
  invCam = invCam * Scene::world->getMatrix();
  Vector3 camPos3 = Scene::camera->getPos();
  Vector4 camPos = invCam * Vector4( camPos3.getX(), camPos3.getY(), camPos3.getZ(), 1.0 );

  // Draw our scene so long as it is actually in memory
  if ( Scene::camera && Scene::world ) {

	// Enable environment mapping on our patch
	if (Scene::shaderOn) {
		Scene::shader->bind();
		// glBindTexture(GL_TEXTURE_2D, 0); // unbind textures?
		glBindTexture(GL_TEXTURE_2D, Scene::sky->front);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Draw the patch et al
	Matrix4 invCam2 = Scene::camera->getGLMatrix();
	Scene::world->draw( invCam2 );
	if ( Scene::shaderOn ) {
		Scene::shader->unbind(); // Unbind after drawing here
	}

	// Draw the skybox without shader interference
	Scene::skyBoxScale->draw(invCam);
	
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
  case 'e':
	  Scene::shaderOn = !Scene::shaderOn;
	  cerr << "Environment mapping is " << (Scene::shaderOn == true ? "on" : "off") << endl;
	  break;
  case 'f':
	  Scene::showFps = !Scene::showFps;
	  cerr << "FPS counter is " << (Scene::showFps ? "on" : "off") << endl;
	  break;
/**
  case 'x': // moving camera in -x direction, requires moving world in +x
	  transformation = Matrix4::translate(1.0, 0.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'X':
	  transformation = Matrix4::translate(-1.0, 0.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'y':
	  transformation = Matrix4::translate(0.0, +1.0, 0.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'Y':
	  transformation = Matrix4::translate(0.0, -1.0, 0.0);
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
*/
  case 'w':
      Scene::camera->lookAt(1, 1);
      break;
  case 'a':
      Scene::camera->lookAt(1, 0);
      break;
  case 's':
      Scene::camera->lookAt(0, 1);
      break;
  case 'd':
      Scene::camera->lookAt(0, 0);
      break;
  case 'z':
      Scene::camera->moveForward();
      break;
  case 'x':
      Scene::camera->moveBackward();
      break;
  case 'o':
	  transformation = Matrix4::rotY(-5.0);
	  Scene::world->getMatrix().transformWorld(transformation);
	  break;
  case 'O':
	  transformation = Matrix4::rotY(+5.0);
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

/** Load a ppm file from disk, then loads it into OpenlGL.
 @input filename The location of the PPM file.  If the file is not found, an error message
		will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured

 Stolen from Jurgen
**/
GLuint Window::loadPPM(const char *filename, int width, int height, int texID) {

	// GLuint texture[1];     // storage for one texture
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData; // texture pixel data
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
	  retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	// Load this file into an OpenGL texture object!
  
	// Create ID for texture
	// glGenTextures(1, &texture[0]);   

	// Set this texture to be the one we are working with
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	GLuint texLoc;
	switch (texID) {
		case 0: texLoc = glGetUniformLocationARB(Scene::shader->pid,"right"); break;
		case 1: texLoc = glGetUniformLocationARB(Scene::shader->pid,"left"); break;
		case 2: texLoc = glGetUniformLocationARB(Scene::shader->pid,"front"); break;
		case 3: texLoc = glGetUniformLocationARB(Scene::shader->pid,"back"); break;
		case 4: texLoc = glGetUniformLocationARB(Scene::shader->pid,"top"); break;
		case 5: texLoc = glGetUniformLocationARB(Scene::shader->pid,"base"); break;
	}

	int err = glGetError();
	if (err != GL_NO_ERROR) cerr <<  "in loadPPM: after get uniform location: " << gluErrorString( err ) << endl;

	glUniform1i( texLoc, 0 );

	err = glGetError();
	if (err != GL_NO_ERROR) cerr << "in loadPPM: after uniform1i: " << gluErrorString( err ) << endl;
		
	glActiveTexture(GL_TEXTURE0);
	// switch (texID) {
	// 	case 0: glActiveTexture( GL_TEXTURE0 ); break; // right
	// 	case 1: glActiveTexture( GL_TEXTURE1 ); break; // left
	// 	case 2: glActiveTexture( GL_TEXTURE2 ); break; // front
	// 	case 3: glActiveTexture( GL_TEXTURE3 ); break; // back
	// 	case 4: glActiveTexture( GL_TEXTURE4 ); break; // top
	// 	case 5: glActiveTexture( GL_TEXTURE5 ); break; // base
	// }
	glBindTexture(GL_TEXTURE_2D, Scene::textures[texID]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// cerr << filename << " has tex ID: " << texture[0] << endl;
	// return texture[0];
	cerr << filename << " has tex ID: " << Scene::textures[texID] << endl;
	return Scene::textures[texID];
}

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
