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
	normal = Vector3( n ); // Copy ctor
	normal.normalize(); // Don't trust the user
	point = Vector3( p ); // Copy ctor
	precomputed = p.dot(p, n);
}

Plane::~Plane()
{
}

bool Plane::sphereInsideOrOn(Vector3 center, double rad) {
	double distance = center.dot(center, normal) + precomputed;
	if (distance < -rad) {
		return Plane::OUTSIDE;
	}
	else if (distance < rad) {
		return Plane::INTERSECTS;
	}
	else {
		return Plane::INSIDE;
	}
}