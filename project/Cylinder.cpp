#include "Cylinder.h"
#include "Util.h"

Cylinder::Cylinder(GLUquadricObj * q)
{
  top_r = bot_r = 0.5;
	height = 1;
	boundingRadius = Util::max(Util::max(top_r,bot_r), height/2.0);
	longitude = latitude = 16; // 32 is way too high
  quad = q;
}

Cylinder::Cylinder(GLUquadricObj * q, double b, double t, double h)
{
  bot_r = b;
  top_r = t;
	height = h;
	boundingRadius = Util::max(Util::max(top_r,bot_r), height/2.0);
	longitude = latitude = 16; // 32 is way too high
  quad = q;
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
  /*
  glTranslatef(0,0,1);
  gluDisk(quad,0,top_r,longitude,latitude);

  glTranslatef(0,0,-1);
  gluQuadricOrientation(quad,GLU_INSIDE);
  gluDisk(quad,0,bot_r,longitude,latitude);
  */
  glPopMatrix();
}
