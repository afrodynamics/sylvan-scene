#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <vector>
#include "Matrix4.h"
#include "Camera.h"
#include "Robot.h"
#include "Plane.h"
#include "ObjModel.h"

/**
 * Keeps all our scene data together
 */
namespace Scene
{
	extern Camera *camera;
	extern std::vector<Node*> nodeList;
	extern double znear, zfar;
	extern MatrixTransform *world;
	extern Robot* createRobot(Vector3&);
	extern void setup();
	extern void dealloc();
};

class Window	  // OpenGL output window related routines
{
  public:
    static int width, height;  // useful vars
    static double fov, deltaTime;

	// Courtesy of http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
	static GLuint loadPPM(const char *filename, int width, int height, int texID);

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void keyboardCallback(unsigned char,int,int);
    static void functionKeysCallback(int,int,int);

    // Draw Methods
    static void displayCallback(void);

};

#endif

