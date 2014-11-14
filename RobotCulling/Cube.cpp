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
	scale = Vector4(sideLength / 2, sideLength / 2, sideLength / 2, 1);
	boundingRadius = (Vector3(sideLength/2,sideLength/2,sideLength/2)).length();
}


Cube::~Cube()
{
}

void Cube::render() {
	if (drawBoundingSphere == true) {
		
		scale = Vector4(sideLength / 2, sideLength / 2, sideLength / 2, 0);

		Matrix4 tmp = Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ()) *
			Matrix4::scale(boundingRadius,boundingRadius,boundingRadius);
		tmp.transpose();
		glPushMatrix();
		glLoadMatrixd(tmp.getPointer());
		glutWireSphere(1, 10, 10);
		glPopMatrix();

	}
	glutSolidCube(sideLength);
}