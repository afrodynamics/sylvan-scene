#pragma once
#include "Vector3.h"

class Plane
{
public:
	double precomputed;
	static int INSIDE, OUTSIDE, INTERSECTS;
	Vector3 normal, point;
	Plane();
	Plane(Vector3 n, Vector3 p);
	~Plane();
	bool sphereInsideOrOn(Vector3 center, double radius);
};

