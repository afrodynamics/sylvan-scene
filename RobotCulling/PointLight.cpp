#include "PointLight.h"
#include "Wiresphere.h"

PointLight::PointLight()
{
	positionSphere = new Wiresphere();
	lightIndex = Node::lightCounter;

	spotAngle = 180.0;
	spotExponent = 0.0;
	spotDir[0] = 0.0;
	spotDir[1] = 0.0;
	spotDir[2] = -1.0;

	// Set light properties to OpenGL default

	setPosition(0, 0, 0, 1);
	setAmbient(0, 0, 0, 1);
	setDiffuse(1, 1, 1, 1);
	setSpecular(1, 1, 1, 1);

	Node::lightCounter++;
}

/**
 * Point lights are always in world coordinates
 * because in PointLight::draw() we pop the GL 
 * state and load the identity
 */
PointLight::PointLight(double x, double y, double z)
{
	positionSphere = new Wiresphere();
	centerPos = Vector4( x, y, z, 1 );
	setPosition(x, y, z, 1 );
	setAmbient(0, 0, 0, 1);
	setDiffuse(1, 1, 1, 1);
	setSpecular(1, 1, 1, 1);

	spotAngle = 180.0;
	spotExponent = 0.0;
	spotDir[0] = 0.0;
	spotDir[1] = 0.0;
	spotDir[2] = -1.0;

	lightIndex = Node::lightCounter;
	Node::lightCounter++;
}

PointLight::~PointLight()
{
	delete positionSphere;
	Node::lightCounter--;
}

void PointLight::setPosition(float x, float y, float z, float w) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = w; // All positional light sources have nonzero positions
	centerPos = Vector4(x, y, z, 1);
}

void PointLight::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}

void PointLight::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void PointLight::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void PointLight::render() {

	GLenum lightId;
	switch (lightIndex) {
	case 0: lightId = GL_LIGHT0; break;
	case 1: lightId = GL_LIGHT1; break;
	case 2: lightId = GL_LIGHT2; break;
	case 3: lightId = GL_LIGHT3; break;
	case 4: lightId = GL_LIGHT4; break;
	case 5: lightId = GL_LIGHT5; break;
	case 6: lightId = GL_LIGHT6; break;
	case 7: lightId = GL_LIGHT7; break;
	default:
		break; // Too many lights
	}

	centerPos = Vector4(position[0], position[1], position[2], position[3]);
	Matrix4 mtx = Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ());
	centerPos = mtx * centerPos;
	mtx.transpose();

	//float transformPos[] = { centerPos.getX(), centerPos.getY(), centerPos.getZ(), centerPos.getW() };

	glEnable(lightId); // Enable this light, duh!
	glPushMatrix();    // Ignore the last state
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( mtx.getPointer() );  // Load the identity, since lights aren't relative to the camera
	if (spotAngle != 180.0) {
		glGetLightfv(lightId, GL_SPOT_CUTOFF, &spotAngle);
		glGetLightfv(lightId, GL_SPOT_DIRECTION, spotDir );
		glGetLightfv(lightId, GL_SPOT_EXPONENT, &spotExponent );
	}
	else {
		glLightfv(lightId, GL_AMBIENT, ambient);
		glLightfv(lightId, GL_DIFFUSE, diffuse);
		glLightfv(lightId, GL_SPECULAR, specular);
	}
	glLightfv( lightId, GL_POSITION, position );
	
	positionSphere->render();
	
	glPopMatrix();

};