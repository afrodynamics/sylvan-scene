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
	double latitude, longitude, top_r, bot_r, height;
  GLUquadricObj * quad;

public:
	Cylinder(GLUquadricObj * q);
	Cylinder(GLUquadricObj * q, double b, double t, double h);
	Cylinder(GLUquadricObj * q, double b, double t, double h, double lat, double lon);
	~Cylinder();
	void render();
};