#pragma once
#include "Matrix4.h"
#include "shader.h"

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
	bool culling, useShader;
	Shader *shader;

	// Light Index (for *Light instances)
	int lightIndex;

public:	
	static int lightCounter; // Light subclasses will inc/dec this counter when they are allocated/deallocated

	Vector4 centerPos, scale; // These points are in camera coordinates
	double boundingRadius; // This distance is in camera space, and is calculated at runtime
	double minX, maxX, minY, maxY, minZ, maxZ;
	bool drawBoundingSphere;
	Node();
	~Node();
	virtual void draw(Matrix4& C) = 0;
	virtual void showBoundingBox(bool) = 0;
	virtual void setShader(Shader*) = 0; // Attach shader to this node
	virtual void enableShader(bool) = 0; // Enable/disable shader

};

