#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <vector>
#include "Matrix4.h"
#include "Camera.h"
#include "Robot.h"
#include "Plane.h"

/**
 * Keeps all our scene data together
 */
namespace Scene
{
	extern Camera *camera;
	extern std::vector<Node*> nodeList;
	extern std::vector<Plane> frustumPlanes;
	extern double znear, zfar;
	extern Group *world;
	extern Robot* createRobot(Vector3&);
	extern void setup();
	extern void dealloc();
};

class Window	  // OpenGL output window related routines
{
  public:
    static int width, height; 	            // window size
    static int cameraMode;
    static double fov;

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void keyboardCallback(unsigned char,int,int);
    static void functionKeysCallback(int,int,int);

    // Draw Methods
    static void displayCallback(void);

};

#endif
