#pragma once
#include "Matrix4.h"
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include <GL/glut.h>    // Windows and Linux are normal
#endif

class Node
{
protected:
	Matrix4 lastC; // Last matrix we received in the draw method, multiplied by the identity
	               // or by the matrix stored in this node (if it is a MatrixTransform)
public:
	Vector4 centerPos, maxSpherePoint; // These points are in camera coordinates
	double boundingRadius; // This distance is in local space, and is calculated at runtime
	bool drawBoundingSphere;
	Node();
	~Node();
	virtual void draw(Matrix4& C) = 0;
	virtual void showBoundingBox(bool) = 0;

	// Recursively updates the bounding sphere information according to the last matrix
	// we received in draw (transformed by an auxilary matrix if the given Node has its
	// own matrix.)
	virtual void updateBounds(void) = 0; 
};

