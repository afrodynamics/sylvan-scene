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
	scale = Vector4(radius, radius, radius, 1);
}

Sphere::~Sphere()
{
}

void Sphere::render() {
	if (drawBoundingSphere == true) {	
		Matrix4 tmp = Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ()) *
			Matrix4::scale(scale.getX(), scale.getY(), scale.getZ());
		tmp.transpose();
		glPushMatrix();
		glLoadMatrixd(tmp.getPointer());
		glutWireSphere(1, 10, 10);
		glPopMatrix();
	}
	glutSolidSphere(radius, longitude, latitude);
}