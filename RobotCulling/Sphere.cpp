#include "Sphere.h"

Sphere::Sphere()
{
	boundingRadius = radius = 1;
	longitude = latitude = 8;
	lastC.identity();
}

Sphere::Sphere(double rad, double lon, double lat)
{
	boundingRadius = radius = rad; longitude = lon; latitude = lat;
}

Sphere::~Sphere()
{
}

void Sphere::render() {
	if (drawBoundingSphere == true) {	
	//	glutWireSphere(boundingRadius, 10, 10);
	}
	glutSolidSphere(radius, longitude, latitude);
}