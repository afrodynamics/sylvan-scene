#include "Sphere.h"

Sphere::Sphere()
{
	boundingRadius = radius = 1;
	longitude = latitude = 16;
	lastC.identity();
	scale = Vector4(radius, radius, radius, 1);
}

Sphere::~Sphere()
{
}

void Sphere::render() {
	if (drawBoundingSphere == true) {	

		scale = lastC * Vector4(.5, .5, .5, 0);
		boundingRadius = scale.length();
		Matrix4 tmp = Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ()) *
			Matrix4::scale(boundingRadius, boundingRadius, boundingRadius);
		tmp.transpose();
		glPushMatrix();
		glLoadMatrixd(tmp.getPointer());
		glutWireSphere(1, 12, 12);
		glPopMatrix();

	}
	glutSolidSphere(radius, longitude, latitude);
}