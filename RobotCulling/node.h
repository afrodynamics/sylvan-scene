#pragma once
#include "Matrix4.h"
#include "Plane.h"
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include <GL/glut.h>    // Windows and Linux are normal
#endif
#include <vector>
#include <float.h>

class Node
{
protected:
	Matrix4 lastC; // Last matrix we received in the draw method, multiplied by the identity
	               // or by the matrix stored in this node (if it is a MatrixTransform)
	bool culling;
	std::vector<Plane> *frustumPlanes;
public:
	Vector4 centerPos, maxSpherePoint; // These points are in camera coordinates
	double boundingRadius; // This distance is in camera space, and is calculated at runtime
	double minX, maxX, minY, maxY, minZ, maxZ;
	bool drawBoundingSphere;
	Node();
	~Node();
	virtual void draw(Matrix4& C) = 0;
	virtual void showBoundingBox(bool) = 0;
	virtual void setCulling(bool) = 0;

	virtual void setVector(std::vector<Plane> *f) = 0;

	// Recursively updates the bounding sphere information according to the last matrix
	// we received in draw (transformed by an auxilary matrix if the given Node has its
	// own matrix.)
	virtual void updateBounds(void) = 0; 
};

