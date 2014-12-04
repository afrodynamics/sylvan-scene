#include <iostream>
#include "SkyBox.h"

using namespace std;

SkyBox::SkyBox()
{
	sideLength = 1;
	boundingRadius = (Vector3(sideLength / 2, sideLength / 2, sideLength / 2)).length();
	lastC.identity();
}

SkyBox::~SkyBox()
{
}

void SkyBox::render() {
	/**
	 * Face Semantics
	 *
	 * Front	- positive Z, XY plane
	 * Back		- negative Z, XY plane
	 * Top		- positive Y, XZ plane
	 * Base		- negative Y, XZ plane
	 * Left		- negative X, YZ plane
	 * Right 	- positive X, YZ plane
	 *
	 * All faces are unit length, with cube centered at 0,0,0
	 * so that the scene graph positioning isn't a giant pain
	 */

	// Back Face
	
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D, back);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(0,0);
	glVertex3f(-.5,-.5,-.5); // bottom left - from testing
	
	glTexCoord2f(0,1);
	glVertex3f(+.5,-.5,-.5); // bottom right - from testing

	glTexCoord2f(1,1);
	glVertex3f(+.5,+.5,-.5); // top right corner - from testing

	glTexCoord2f(1,0);
	glVertex3f(-.5,+.5,-.5); // top left corner - from testing (1,0)

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
	
	// Front Face

	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);
	// glTexCoord2f(1,1);
	glTexCoord2f(1,0);
	glVertex3f(-.5,-.5,+.5);
	
	// glTexCoord2f(0,0);
	glTexCoord2f(0,1);
	glVertex3f(-.5,+.5,+.5);
	
	// glTexCoord2f(1,0);
	glTexCoord2f(0,0);
	glVertex3f(+.5,+.5,+.5);

	// glTexCoord2f(0,1);
	glTexCoord2f(1,1);
	glVertex3f(+.5,-.5,+.5);

	glEnd();	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	// Left Face (well actually it's on the right because everything is broken)

	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(1,1);
	glVertex3f(-.5, -.5, -.5);
	
	glTexCoord2f(1,0);
	glVertex3f(-.5, +.5, -.5);
	
	glTexCoord2f(0,1);
	glVertex3f(-.5, +.5, +.5);

	glTexCoord2f(0,0);
	glVertex3f(-.5, -.5, +.5);

	glEnd();	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	// Right Face (well actually it's on the left because everything is broken)

	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);

	glTexCoord2f(0,0);
	glVertex3f(+.5, -.5, +.5);
	
	glTexCoord2f(0,1);
	glVertex3f(+.5, +.5, +.5);

	glTexCoord2f(1,0);
	glVertex3f(+.5, +.5, -.5);

	glTexCoord2f(1,1);
	glVertex3f(+.5, -.5, -.5);

	glEnd();	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	// Bottom Face (Ground)
	
	glBindTexture(GL_TEXTURE_2D, base);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);

	glTexCoord2f(1,1);
	glVertex3f(-.5, -.5, +.5);
	
	glTexCoord2f(1,0);
	glVertex3f(+.5, -.5, +.5);
	
	glTexCoord2f(0,1);
	glVertex3f(+.5, -.5, -.5);

	glTexCoord2f(0,0);
	glVertex3f(-.5, -.5, -.5);
	
	glEnd();	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	// Top Face
	
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0,1.0,0.0);
	
	glTexCoord2f(0,0);
	glVertex3f(-.5, +.5, -.5);
	
	glTexCoord2f(0,1);
	glVertex3f(+.5, +.5, -.5);

	glTexCoord2f(1,0);
	glVertex3f(+.5, +.5, +.5);	

	glTexCoord2f(1,1);
	glVertex3f(-.5, +.5, +.5);
	
	glEnd();	
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

}