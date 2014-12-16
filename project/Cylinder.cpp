#include "Cylinder.h"

/* Constructor
 * q - GLUQuadratic object.  Borrowed, do not delete
 */
Cylinder::Cylinder(GLUquadricObj * q) : Cylinder(q,0.5,0.5,1, 16, 16)
{
}

/* Constructor
 * q - GLUQuadratic object.  Borrowed, do not delete
 * b - bottom radius
 * t - top radius
 * h - height
 */
Cylinder::Cylinder(GLUquadricObj * q, double b, double t, double h) :
  Cylinder(q,b,t,h,16,16)
{
}

/* Constructor
 * q - GLUQuadratic object.  Borrowed, do not delete
 * b - bottom radius
 * t - top radius
 * h - height
 * lat - latitude 
 */
Cylinder::Cylinder(GLUquadricObj * q, double b, double t, double h,
                      double lat, double lon)
{
  bot_r = b;
  top_r = t;
	height = h;
	boundingRadius = max(max(top_r,bot_r), height/2.0);
  latitude = lat;
	longitude = lon;
  quad = q;
}


Cylinder::~Cylinder()
{
  // No need to delete GLUQuadratic since it is borrowed
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
