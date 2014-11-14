#pragma once
#include "Vector3.h"
#define INSIDE 0
#define INTERSECTS 1
#define OUTSIDE 2

class Plane
{
private:
	double precomputed;
public:
	Vector3 normal, point;
	Plane();
	Plane(Vector3 n, Vector3 p);
	~Plane();
	bool sphereInsideOrOn(Vector3 center, double radius);
};

