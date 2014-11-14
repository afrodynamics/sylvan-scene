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

	lastC = C * *mtx;
	centerPos = lastC * Vector4(0,0,0,1);
	if (!culling) {
		Group::draw(lastC);
	}
	else {
		// Do intersection testing here
		if (frustumPlanes != nullptr) {
			bool inside = false;
			for (auto iter = frustumPlanes->begin(); iter != frustumPlanes->end(); iter++) {
				inside = inside || (*iter).sphereInsideOrOn(Vector3(centerPos.getX(), centerPos.getY(), centerPos.getZ()), boundingRadius);
				if (inside == true) break;
			}
			if (inside == true) {
				// If we are inside the frustum, draw
				Group::draw(lastC);
			}
		}
	}

}

// Returns a reference to the matrix stored in this node
Matrix4& MatrixTransform::getMatrix() {
	return *mtx;
}

void MatrixTransform::updateBounds(void) {

	// This works in Robot/MatrixTransform since we set
	// lastC = C * *mtx
	// in those classes' draw methods

	Vector4 center = lastC * Vector4(0, 0, 0, 1); // Get our origin in this camera coordinate space
	double maxL = -1;

	// Go through all of the extreme points in our children and apply all of the transformations
	// Whichever point is furthest from our center point is our maximal radius
	// and therefore should be used for culling. For now, we'll save the extreme
	// point in maxSpherePoint

	for (auto iter = children.begin(); iter != children.end(); iter++) {

		// The magic of recursion:
		//   Update the positions of the most extreme sphere points
		// in world coordinates of all of the children
		(*iter)->updateBounds();

		Node *curr = (*iter);

		if (curr->minX < minX) {
			minX = curr->minX;
		}
		if (curr->maxX > maxX) {
			maxX = curr->maxX;
		}
		if (curr->minY < minY) {
			minY = curr->minY;
		}
		if (curr->maxY > maxY) {
			maxY = curr->maxY;
		}
		if (curr->minZ < minZ) {
			minZ = curr->minZ;
		}
		if (curr->maxZ > maxZ) {
			maxZ = curr->maxZ;
		}

	}

	boundingRadius = 0;

};

void MatrixTransform::setVector(std::vector<Plane> *f) {
	frustumPlanes = f;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->setVector(f);
		iter++;
	}
}