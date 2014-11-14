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

	Group::draw(lastC);

}

void MatrixTransform::cdraw(Matrix4& C) {

	lastC = C * *mtx;
	centerPos = lastC * Vector4(0, 0, 0, 1);

	{
		// Do intersection testing here
		if (frustumPlanes != nullptr) {
			bool inside = false;
			bool intersects = false;
			Vector3 center = Vector3(centerPos.getX(), centerPos.getY(), centerPos.getZ());

			for (auto iter = frustumPlanes->begin(); iter != frustumPlanes->end(); iter++) {

				Vector3 center = Vector3(centerPos.getX(), centerPos.getY(), centerPos.getZ());
				double distance = center.dot(center, (*iter).normal) - (*iter).precomputed;
				if (distance > boundingRadius) {
					// The distance will be positive if the sphere is on the side of the plane
					// facing the normal, or 0 if it is on the plane
					inside = true;
				}
				else if ( distance >= -boundingRadius ) {
					intersects = true;
				}
				else {
					intersects = false;
					inside = false;
				}

			}
			
			if (intersects == true) {
				Group::draw(lastC);
			}
			else if (inside == true) {
				Group::draw(lastC);
			}
			else {
				return;
			}

		}
	}

	updateBounds();

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

	boundingRadius = (Vector3(maxX,maxY,maxZ)).length();

};

void MatrixTransform::setVector(std::vector<Plane> *f) {
	frustumPlanes = f;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->setVector(f);
		iter++;
	}
}