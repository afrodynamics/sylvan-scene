#pragma once
#include "Geode.h"

/* Cylinder class that draws a cylinder with height = 1, diameter = 1.
 * The cylinder is drawn with the base at the origin, cylinder drawn upward,
 * like a tree trunk
 */
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