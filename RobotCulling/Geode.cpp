#include <vector>
#include <cmath>
#include <iostream>
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

	if ( culling == false ) {
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
			bool intersects = false;
			for (auto iter = frustumPlanes->begin(); iter != frustumPlanes->end(); iter++) {
				inside = inside || (*iter).sphereInsideOrOn(Vector3(centerPos.getX(), centerPos.getY(), centerPos.getZ()), boundingRadius);
			}
			if (inside == true || intersects == true) {
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

	Vector4 center = lastC * Vector4(0, 0, 0, 1); // Get our origin in world coordinates
	scale = Vector4(.5,.5,.5,0);
	boundingRadius = scale.length();

}