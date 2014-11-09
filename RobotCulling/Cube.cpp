#include "Cube.h"


Cube::Cube()
{
	sideLength = 1;
}

Cube::Cube(double l)
{
	sideLength = l;
}


Cube::~Cube()
{
}

void Cube::render() {
	glutSolidCube(sideLength);
}