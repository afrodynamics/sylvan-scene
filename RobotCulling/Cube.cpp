#include "Cube.h"


Cube::Cube()
{
	sideLength = 1;
	boundingRadius = sideLength;
}

Cube::Cube(double l)
{
	sideLength = l;
	boundingRadius = sideLength;
}


Cube::~Cube()
{
}

void Cube::render() {
	glutSolidCube(sideLength);
}