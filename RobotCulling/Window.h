#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "Matrix4.h"

/**
   We shouldn't technically need this class anymore for the project 3 but maybe...?
 */

class Window	  // OpenGL output window related routines
{
  public:
    static int width, height; 	            // window size
    static bool ball;                       // ball/cube mode
    static int spinDirection;               // spin direction in cube mode
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

