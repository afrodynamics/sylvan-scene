#include "Plane.h"

int Plane::INSIDE = 60;
int Plane::OUTSIDE = 61;
int Plane::INTERSECTS = 62;

Plane::Plane()
{
	normal = point = Vector3(0, 0, 0);
}

Plane::Plane(Vector3 n, Vector3 p)
{
	normal = n;
	normal.normalize(); // Don't trust the user
	point = p;
	precomputed = p.dot(p, n);
}

Plane::~Plane()
{
}

bool Plane::sphereInsideOrOn(Vector3 center, double rad) {
	double distance = center.dot(center, normal) + precomputed;
	if (distance > -rad) {
		// The distance will be positive if the sphere is on the side of the plane
		// facing the normal, or 0 if it is on the plane
		return Plane::INSIDE;
	}
	else if (distance >= -rad) {
		return Plane::INTERSECTS;
	}
	else {
		return Plane::OUTSIDE;
	}
}