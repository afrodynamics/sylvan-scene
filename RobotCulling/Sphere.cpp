#include "Sphere.h"


Sphere::Sphere()
{
	radius = 1;
	longitude = latitude = 8;
}

Sphere::Sphere(double rad, double lon, double lat)
{
	radius = rad; longitude = lon; latitude = lat;
}

Sphere::~Sphere()
{
}

void Sphere::render() {
	glutSolidSphere(radius, longitude, latitude);
}