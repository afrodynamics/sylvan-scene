#pragma once
#include "Geode.h"
class Wiresphere :
	public Geode
{
private:
	double radius, longitude, latitude;
public:
	Wiresphere();
	Wiresphere(double, double, double); // radius, longitude divisions, latitude divisions
	~Wiresphere();
	void render();
};

