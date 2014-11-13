#include <vector>
#include "Geode.h"

using namespace std;

// Constructor
Geode::Geode()
{
	drawBoundingSphere = false;
	maxSpherePoint = Vector4(0, 0, 0, 1);
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
	tmp.transpose();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( tmp.getPointer() );
	render();
}

// Toggles showing the bounding sphere around this object
void Geode::showBoundingBox(bool show) {
	this->drawBoundingSphere = show;
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

	// Create a list of 6 vertices around the extremes of the bounding
	// sphere, and convert them to camera/world coordinates

	vector<Vector4> list;
	list.push_back(lastC * Vector4(boundingRadius,0,0,1) );
	list.push_back(lastC * Vector4(-boundingRadius, 0, 0, 1));
	list.push_back(lastC * Vector4(0, boundingRadius, 0, 1));
	list.push_back(lastC * Vector4(0, -boundingRadius, 0, 1));
	list.push_back(lastC * Vector4(0, 0, boundingRadius, 1));
	list.push_back(lastC * Vector4(0, 0, -boundingRadius, 1));

	double max = -1;

	// Go through all of the extreme points and apply all of the transformations
	// Whichever point is furthest from our center point is our maximal radius
	// and therefore should be used for culling. For now, we'll save the extreme
	// point in maxSpherePoint

	for (auto iter = list.begin(); iter != list.end(); iter++) {
		// Center in local coords is always 0,0,0
		if ((*iter).length() > max) {
			max = (*iter).length();
			maxSpherePoint = lastC * *iter;
		}
	}
}