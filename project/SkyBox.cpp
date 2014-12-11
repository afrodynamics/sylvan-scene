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
	 * 
	 * OpenGL texture coordinates are centered in the upper left
	 * +----------+
	 * | (0,0)    | (1,0)
	 * |          |
	 * |          |
	 * |          |
	 * +----------+ (1,1)
	 *   (0,1)
	 */

	// Front Face  ( GOOD! )
	
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(0,1);
	glVertex3f(-.5,-.5,-.5); // bottom left - from testing
	
	glTexCoord2f(1,1);
	glVertex3f(+.5,-.5,-.5); // bottom right - from testing

	glTexCoord2f(1,0);
	glVertex3f(+.5,+.5,-.5); // top right corner - from testing

	glTexCoord2f(0,0);
	glVertex3f(-.5,+.5,-.5); // top left corner - from testing (1,0)

	glEnd();
	
	// Back Face  ( GOOD, SEMANTICALLY AND IN GAME! )

	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(1,1);
	glVertex3f(-.5,-.5,+.5); // bottom right (when looking straight at it)
	
	glTexCoord2f(1,0);
	glVertex3f(-.5,+.5,+.5); // top right
	
	glTexCoord2f(0,0);
	glVertex3f(+.5,+.5,+.5); // top left

	glTexCoord2f(0,1);
	glVertex3f(+.5,-.5,+.5); // bot left

	glEnd();	

	// Left Face ( GOOD ! )

	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
							// glColor3f(1,0,0);
	glNormal3f(0.0,1.0,0.0);
	glTexCoord2f(1,1);
	glVertex3f(-.5, -.5, -.5); // bot right
	
	glTexCoord2f(1,0);
	glVertex3f(-.5, +.5, -.5); // top right (looking straight at this face)
	
	glTexCoord2f(0,0);
	glVertex3f(-.5, +.5, +.5); // top left

	glTexCoord2f(0,1);
	glVertex3f(-.5, -.5, +.5); // bot left
							// glColor3f(1,1,1);
	glEnd();	

	// Right Face ( GOOD ! )

	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);

	glTexCoord2f(1,1);
	glVertex3f(+.5, -.5, +.5); // bot left (looking straight at the face)
	
	glTexCoord2f(1,0);
	glVertex3f(+.5, +.5, +.5); // top left

	glTexCoord2f(0,0);
	glVertex3f(+.5, +.5, -.5); // top right

	glTexCoord2f(0,1);
	glVertex3f(+.5, -.5, -.5); // bot right

	glEnd();	

	// Bottom Face ( GOOD ! )
	
	glBindTexture(GL_TEXTURE_2D, base);
	glBegin(GL_QUADS);

	glNormal3f(0.0,1.0,0.0);

	glTexCoord2f(0,1);
	glVertex3f(-.5, -.5, +.5); // bot left (looking straight down at the floor)
	
	glTexCoord2f(1,1);
	glVertex3f(+.5, -.5, +.5); // bot right
	
	glTexCoord2f(1,0);
	glVertex3f(+.5, -.5, -.5); // top right

	glTexCoord2f(0,0);
	glVertex3f(-.5, -.5, -.5); // top left
	
	glEnd();	

	// Top Face ( GOOD ! )
	
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0,1.0,0.0);
	
	glTexCoord2f(0,1);
	glVertex3f(-.5, +.5, +.5); 
	
	glTexCoord2f(1,1);
	glVertex3f(-.5, +.5, -.5);

	glTexCoord2f(1,0);
	glVertex3f(+.5, +.5, -.5);

	glTexCoord2f(0,0);
	glVertex3f(+.5, +.5, +.5);
	
	glEnd();	
	//glBindTexture(GL_TEXTURE_2D, 0); // Unbind all textures

	// glDisable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

}