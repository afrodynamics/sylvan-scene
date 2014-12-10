#include "Wiresphere.h"


Wiresphere::Wiresphere()
{
	boundingRadius = radius = 1;
	longitude = latitude = 8;
	lastC.identity();
}

Wiresphere::Wiresphere(double rad, double lon, double lat)
{
	boundingRadius = radius = rad; longitude = lon; latitude = lat;
}

Wiresphere::~Wiresphere()
{
}

void Wiresphere::render() {
	glutWireSphere(radius, longitude, latitude);
}