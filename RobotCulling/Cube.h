#pragma once
#include "Geode.h"
class Cube :
	public Geode
{
public:
	double sideLength;
	Cube();
	Cube(double);
	~Cube();
	void render();
};

