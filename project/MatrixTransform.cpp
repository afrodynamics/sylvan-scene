#include <iostream>
#include "Vector4.h"
#include "MatrixTransform.h"

using namespace std;

// Default constructor
MatrixTransform::MatrixTransform()
{
	mtx.identity();
}

// Constructor: pass by value (so cland will compile stuff VS normally would)
MatrixTransform::MatrixTransform(Matrix4 matrix)
{
	mtx = Matrix4(matrix);
}

// destructor
MatrixTransform::~MatrixTransform() {}

// Applies the transformation stored in this Node and calls
// draw on all the children with the resulting matrix
void MatrixTransform::draw(Matrix4& C) {

	lastC = C * mtx;
	lastC.transpose();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(lastC.getPointer()); // Load the matrix now

	lastC.transpose();
	Group::draw(lastC);	
	//centerPos = lastC * Vector4(0,0,0,1);

}

// Returns a reference to the matrix stored in this node
Matrix4& MatrixTransform::getMatrix() {
	return mtx;
}