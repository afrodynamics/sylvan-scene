#include "Cylinder.h"

Cylinder::Cylinder()
{
	boundingRadius = radius = 0.5;
	height = 1;
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
	gluCylinder(quad, radius, radius, height, longitude, latitude);
  
  // Draw cylinder bases
  glTranslatef(0,0,1);
  gluDisk(quad,0,radius,longitude,latitude);

  glTranslatef(0,0,-1);
  gluQuadricOrientation(quad,GLU_INSIDE);
  gluDisk(quad,0,radius,longitude,latitude);
  glPopMatrix();
}
