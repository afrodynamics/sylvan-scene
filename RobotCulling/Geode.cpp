#include "Geode.h"

Geode::Geode()
{
	drawBoundingSphere = false;
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

void Geode::showBoundingBox(bool show) {
	this->drawBoundingSphere = show;
}