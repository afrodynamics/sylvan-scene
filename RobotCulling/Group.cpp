#include <list>
#include <iostream>
#include "Group.h"

using namespace std;

Group::Group()
{
	boundingRadius = 0;
	lastC.identity();
}

Group::~Group()
{
}

void Group::draw(Matrix4& C) {
	list<Node*>::iterator iter = children.begin();
	while (iter != children.end()) {
		(*iter)->draw(C);
		if (drawBoundingSphere) {

			C.transpose();
			glLoadMatrixd(C.getPointer());
			glutWireSphere(boundingRadius,10,10);
			C.transpose();
		}
		iter++;
	}

}

// We aren't going to manage the memory in the list anymore
void Group::addChild(Node* node) {
	if (node == nullptr) {
		throw "Can't add nullptr to a SceneGraph group.";
	}
	children.push_front( node );
};

void Group::removeChild(Node* node) {
	if (node == nullptr) {
		throw "Can't remove nullptr from a SceneGraph group.";
	}
	children.remove(node);
};

void Group::showBoundingBox(bool show) {
	drawBoundingSphere = show;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->showBoundingBox(show);
		iter++;
	}
}

void Group::updateBounds(void) {

	// This works in Robot/MatrixTransform since we set
	// lastC = C * *mtx
	// in those classes' draw methods

	Vector4 center = lastC * Vector4(0,0,0,1); // Get our origin in world coordinates
	double max = -1;

	// Go through all of the extreme points in our children and apply all of the transformations
	// Whichever point is furthest from our center point is our maximal radius
	// and therefore should be used for culling. For now, we'll save the extreme
	// point in maxSpherePoint

	for (auto iter = children.begin(); iter != children.end(); iter++) {

		// The magic of recursion:
		//   Update the positions of the most extreme sphere points
		// in world coordinates of all of the children
		(*iter)->updateBounds();

		Vector4 centerTest = center - (*iter)->centerPos;
		Vector4 sphereTest = center - (*iter)->maxSpherePoint;

		// Center in local coords is always 0,0,0
		if (centerTest.length() > max) {
			max = centerTest.length();
			maxSpherePoint = lastC * (*iter)->centerPos;
		}
		if (sphereTest.length() > max) {
			max = sphereTest.length();
			maxSpherePoint = lastC * (*iter)->maxSpherePoint;
		}
	}

	// In world coordinates, which does not play nicely with 
	// our draw function
	// boundingRadius = max;
	
};