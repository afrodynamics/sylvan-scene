#include "Cube.h"

Cube::Cube()
{
	sideLength = 1;
	boundingRadius = (Vector3(sideLength / 2, sideLength / 2, sideLength / 2)).length();
	lastC.identity();
}

Cube::Cube(double l)
{
	sideLength = l;
	boundingRadius = (Vector3(sideLength/2,sideLength/2,sideLength/2)).length();
}


Cube::~Cube()
{
}

void Cube::render() {
	if (drawBoundingSphere == true) {
		glutWireSphere(boundingRadius, 10, 10);
	}
	glutSolidCube(sideLength);
}