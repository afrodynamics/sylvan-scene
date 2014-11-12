#pragma once
#include "Matrix4.h"
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include <GL/glut.h>    // Windows and Linux are normal
#endif

class Node
{
public:
	Vector3 centerPos;
	double boundingRadius;
	bool drawBoundingSphere;
	Node();
	~Node();
	virtual void draw(Matrix4& C) = 0;
	virtual void showBoundingBox(bool) = 0;
};

