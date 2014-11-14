#pragma once
#include "Geode.h"
class Cube :
	public Geode
{
private:
	double sideLength;
public:
	Cube();
	//Cube(double);
	~Cube();
	void render();
};

