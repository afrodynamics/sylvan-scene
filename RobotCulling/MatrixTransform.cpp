#include <iostream>
#include "Vector4.h"
#include "MatrixTransform.h"

using namespace std;

// Default constructor
MatrixTransform::MatrixTransform()
{
	mtx = new Matrix4();
	mtx->identity();
}

// Constructor: copies the given matrix, allowing us to easily pass in temporaries
MatrixTransform::MatrixTransform(Matrix4& matrix)
{
	mtx = new Matrix4(matrix);
}

// destructor
MatrixTransform::~MatrixTransform()
{
	delete mtx;
}

// Applies the transformation stored in this Node and calls
// draw on all the children with the resulting matrix
void MatrixTransform::draw(Matrix4& C) {

	Matrix4 tmp = C * *mtx;
	Group::draw(tmp);

}

// Returns a reference to the matrix stored in this node
Matrix4& MatrixTransform::getMatrix() {
	return *mtx;
}