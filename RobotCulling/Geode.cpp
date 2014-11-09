#include "Geode.h"


Geode::Geode()
{
}


Geode::~Geode()
{
}

void Geode::render(Matrix4& C) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(Matrix4::transpose(C).getPointer());
}

void Geode::draw(Matrix4& C) {
	Geode::render(C);
}