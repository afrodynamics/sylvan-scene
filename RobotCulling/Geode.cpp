#include "Geode.h"


Geode::Geode()
{
}


Geode::~Geode()
{
}

void Geode::draw(Matrix4& C) {
	Matrix4 tmp = C;
	tmp.transpose();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( tmp.getPointer() );
	render();
}