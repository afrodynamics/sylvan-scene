#pragma once
#include "Geode.h"
class Cone :
	public Geode
{
private:
	double latitude, longitude, radius, height;
public:
	Cone();
	~Cone();
	void render();
};

