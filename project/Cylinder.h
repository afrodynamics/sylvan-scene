#pragma once
#include "Geode.h"
class Cylinder :
	public Geode
{
private:
	double latitude, longitude, radius, height;
  GLUquadricObj * quad;

public:
	Cylinder();
	~Cylinder();
	void render();
};