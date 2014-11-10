#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <vector>
#include "Matrix4.h"
#include "Camera.h"
#include "Robot.h"

/**
 * Keeps all our scene data together
 */
namespace Scene
{
	extern Camera *camera;
	extern std::vector<Node*> nodeList;
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
    static double leftBound, rightBound,
                  topBound, bottomBound;    // invisible bounds for the ball

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void keyboardCallback(unsigned char,int,int);
    static void functionKeysCallback(int,int,int);

    // Draw Methods
    static void displayCallback(void);

};

#endif

