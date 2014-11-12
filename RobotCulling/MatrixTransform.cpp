#include "Vector4.h"
#include "MatrixTransform.h"

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
	Group::draw( tmp );
	if (drawBoundingSphere) {
		tmp.transpose();
		glLoadMatrixd( tmp.getPointer() );
	}

}

// Toggles the drawing of wireframe bounding spheres, used
// to denote the bounds checking for frustum culling
void MatrixTransform::showBoundingBox(bool show) {
	drawBoundingSphere = show;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->showBoundingBox(show);
		iter++;
	}
}

// Returns a reference to the matrix stored in this node
Matrix4& MatrixTransform::getMatrix() {
	return *mtx;
}