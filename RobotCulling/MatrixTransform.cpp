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

	list<Node*>::iterator iter = children.begin();
	while (iter != children.end()) {
		(*iter)->draw(tmp);
		if (drawBoundingSphere) {

			//Vector3 objPos = (*iter)->centerPos;
			//Vector4 pos = *mtx * Vector4(objPos.getX(), objPos.getY(), objPos.getZ(), 1);
			if (boundingRadius < (*iter)->boundingRadius ) {
				boundingRadius = (*iter)->boundingRadius;
			}

		}
		iter++;
	}

	if (drawBoundingSphere) {
		tmp.transpose();
		glLoadMatrixd(tmp.getPointer());
		glutWireSphere(boundingRadius, 10, 10);
		cerr << "boundingRadius " << boundingRadius << endl;
	}

}

// Returns a reference to the matrix stored in this node
Matrix4& MatrixTransform::getMatrix() {
	return *mtx;
}