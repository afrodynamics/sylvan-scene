#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include <GL/glut.h>    // Windows and Linux are normal
#endif
#include "Matrix4.h"
class Node
{
public:
	Node();
	~Node();
	virtual void draw(Matrix4& C) = 0;
};

