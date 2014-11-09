#include "Geode.h"


Geode::Geode()
{
}


Geode::~Geode()
{
}

void Geode::draw(Matrix4& C) {
	glMatrixMode(GL_MODELVIEW);
	render();
}