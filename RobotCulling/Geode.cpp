#include <vector>
#include <cmath>
#include "Geode.h"

using namespace std;

// Constructor
Geode::Geode()
{
	drawBoundingSphere = false;
	scale = Vector4(0, 0, 0, 1);
	centerPos = Vector4(0, 0, 0, 1);
}

// Nothing to destroy
Geode::~Geode()
{
}

// Polymorphic draw method, updates lastC and sets the modelview
// matrix in OpenGL. Assumes C is a row major matrix
void Geode::draw(Matrix4& C) {
	
	Matrix4 tmp = C;
	lastC = C;
	centerPos = C * Vector4(0,0,0,1);

	if ( !culling ) {
		// If we aren't culling, just pass in everything to OpenGL
		tmp.transpose();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd( tmp.getPointer() );
		render();
	}
	else {
		// Do intersection testing here
		if (frustumPlanes != nullptr) {
			bool inside = false;
			for (auto iter = frustumPlanes->begin(); iter != frustumPlanes->end(); iter++) {
				inside = inside || (*iter).sphereInsideOrOn( Vector3( centerPos.getX(), centerPos.getY(), centerPos.getZ() ), boundingRadius );
			}
			if (inside == true) {
				// If we are inside the frustum, draw
				tmp.transpose();
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixd(tmp.getPointer());
				render();
			}
		}
	}

}

// Toggles showing the bounding sphere around this object
void Geode::showBoundingBox(bool show) {
	this->drawBoundingSphere = show;
}

// Toggles culling of this object
void Geode::setCulling(bool show) {
	this->culling = show;
}

// Updates the frustum planes pointer
void Geode::setVector(vector<Plane>* f) {
	this->frustumPlanes = f;
}

/**
 * Updates the maxSpherePoint using the last received matrix.
 * This function is called recursively from the top down if
 * updateBounds() is called on a Group node, so don't call this
 * directly on a Geode unless you have to.
 */
void Geode::updateBounds(void) {

	// boundingRadius is kept in object coordinates, and is
	// calculated in the constructor of Sphere/Cube etc.
	Vector4 center = lastC * Vector4(0, 0, 0, 1); // Get our origin in world coordinates
	boundingRadius = scale.length();
	/*
	// Create a list of 6 vertices around the extremes of the bounding
	// sphere, and convert them to camera/world coordinates
	
	vector<Vector4> list;
	list.push_back(lastC * Vector4(boundingRadius,0,0,1) );
	list.push_back(lastC * Vector4(-boundingRadius, 0, 0, 1));
	list.push_back(lastC * Vector4(0, boundingRadius, 0, 1));
	list.push_back(lastC * Vector4(0, -boundingRadius, 0, 1));
	list.push_back(lastC * Vector4(0, 0, boundingRadius, 1));
	list.push_back(lastC * Vector4(0, 0, -boundingRadius, 1));
	
	double maxL = -1;
	minX = minY = minZ = DBL_MAX;
	maxX = maxY = maxZ = -DBL_MAX;

	// Go through all of the extreme points and apply all of the transformations
	// Whichever point is furthest from our center point is our maximal radius
	// and therefore should be used for culling. For now, we'll save the extreme
	// point in maxSpherePoint

	for (auto iter = list.begin(); iter != list.end(); iter++) {

		Vector4 test = center - *iter;

		if (test.getX() < minX) {
			minX = test.getX();
		}
		if (test.getX() > maxX) {
			maxX = test.getX();
		}
		if (test.getY() < minY) {
			minY = test.getY();
		}
		if (test.getY() > maxY) {
			maxY = test.getY();
		}
		if (test.getZ() < minZ) {
			minZ = test.getZ();
		}
		if (test.getZ() > maxZ) {
			maxZ = test.getZ();
		}
		
		// Center in local coords is always 0,0,0
		if ( (center - *iter).length() > maxL ) {
			maxL = (center - *iter).length();
			scale = *iter;
		}

	}
	*/
	//boundingRadius = Vector3(fmax(fabs(minX), fabs(maxX)), fmax(fabs(minY), fabs(maxY)), fmax(fabs(minZ), fabs(maxZ))).length();
	//boundingRadius = Vector4(0.5,0.5,0.5,0).length();
	//boundingRadius = Vector4(maxX,maxY,maxZ,0).length();

}