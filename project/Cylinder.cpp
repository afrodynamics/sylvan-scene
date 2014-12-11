#include "Cylinder.h"

Cylinder::Cylinder()
{
	boundingRadius = radius = 0.5;
	height = 1;
	longitude = latitude = 32;
  quad = gluNewQuadric();
}


Cylinder::~Cylinder()
{
  gluDeleteQuadric(quad);
}

void Cylinder::render() {
	gluCylinder(quad, radius, radius, height, longitude, latitude);
}
