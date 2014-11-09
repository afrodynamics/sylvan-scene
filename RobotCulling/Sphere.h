#pragma once
#include "Geode.h"
class Sphere :
	public Geode
{
private:
	double radius, longitude, latitude;
public:
	Sphere();
	Sphere(double, double, double); // radius, longitude divisions, latitude divisions
	~Sphere();
	void render();
};

