#include "Sphere.h"


Sphere::Sphere()
{
	boundingRadius = radius = 1;
	longitude = latitude = 8;
}

Sphere::Sphere(double rad, double lon, double lat)
{
	boundingRadius = radius = rad; longitude = lon; latitude = lat;
}

Sphere::~Sphere()
{
}

void Sphere::render() {
	glutSolidSphere(radius, longitude, latitude);
}