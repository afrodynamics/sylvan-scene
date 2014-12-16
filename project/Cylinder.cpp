#include "Cylinder.h"

Cylinder::Cylinder()
{
  top_r = bot_r = 0.5;
	height = 1;
	boundingRadius = max(max(top_r,bot_r), height/2.0);
	longitude = latitude = 32;
  quad = gluNewQuadric();
}

Cylinder::Cylinder(double b, double t, double h)
{
  bot_r = b;
  top_r = t;
	height = h;
	boundingRadius = max(max(top_r,bot_r), height/2.0);
	longitude = latitude = 32;
  quad = gluNewQuadric();
}


Cylinder::~Cylinder()
{
  gluDeleteQuadric(quad);
}

/*
 * The cylinder is drawn with the base at the origin, cylinder drawn upward,
 * like a tree trunk
 */
void Cylinder::render() {
  glPushMatrix();
  glRotatef(90,-1,0,0);
  // Draw cylinder
  gluQuadricOrientation(quad,GLU_OUTSIDE);
	gluCylinder(quad, bot_r, top_r, height, longitude, latitude);
  
  // Draw cylinder bases
  glTranslatef(0,0,1);
  gluDisk(quad,0,top_r,longitude,latitude);

  glTranslatef(0,0,-1);
  gluQuadricOrientation(quad,GLU_INSIDE);
  gluDisk(quad,0,bot_r,longitude,latitude);
  glPopMatrix();
}
