#pragma once
#include "Geode.h"
class SkyBox :
	public Geode
{
private:
	double sideLength;
public:
	GLuint front, back, left, right, top, base; // Textures, no setters
	SkyBox();
	~SkyBox();
	void render();
};

