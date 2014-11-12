#pragma once
#include "Geode.h"
class Sphere :
	public Geode
{
public:
	double radius, longitude, latitude;
	Sphere();
	Sphere(double, double, double); // radius, longitude divisions, latitude divisions
	~Sphere();
	void render();
};

