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
