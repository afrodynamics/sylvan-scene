#include "Plane.h"


Plane::Plane()
{
	normal = point = Vector3(0, 0, 0);
}

Plane::Plane(Vector3 n, Vector3 p)
{
	normal = n;
	point = p;
}

Plane::~Plane()
{
}

bool Plane::sphereInsideOrOn(Vector3 center, double rad) {
	double distance = normal.dot(center - point, normal);
	if (distance >= -rad) {
		// The distance will be positive if the sphere is on the side of the plane
		// facing the normal, or 0 if it is on the 
		return true;
	}
	else {
		return false;
	}
}