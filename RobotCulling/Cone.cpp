#include "Cone.h"


Cone::Cone()
{
	boundingRadius = radius = 0.5;
	height = 1;
	longitude = latitude = 14;
}


Cone::~Cone()
{
}

void Cone::render() {
	glutSolidCone(radius, height, longitude, latitude);
}
