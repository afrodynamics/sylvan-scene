#include "PointLight.h"
#include "Sphere.h"

PointLight::PointLight()
{
	positionSphere = new Sphere();
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
	enabled = true;

	Node::lightCounter++;
}

/**
 * Point lights are always in world coordinates
 * because in PointLight::draw() we pop the GL 
 * state and load the identity
 */
PointLight::PointLight(double x, double y, double z)
{
	positionSphere = new Sphere();
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

	enabled = true;

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

	if (enabled)
		glEnable(lightId); // Enable this light, duh!
	else
		glDisable(lightId);

	// Determine translation matrix and multiply by our camera matrix to position light
	// in the scene
	centerPos = Vector4( position[0], position[1], position[2], position[3] );
	Matrix4 tmp = lastC * Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ());

	if (spotAngle != 180.0) {
		// We want to rotate the cone
		Vector3 sptDir = Vector3( spotDir[0], spotDir[1], spotDir[2] );
		//tmp = tmp * Matrix4::rotate( 0, sptDir );
	}

	tmp.transpose();
	tmp.identity();

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	glEnable(lightId);
	positionSphere->render(); // Geode would've loaded the appropriate matrix
	
	if (spotAngle != 180.0) {
		glLightfv(lightId, GL_AMBIENT, ambient);
		glLightfv(lightId, GL_DIFFUSE, diffuse);
		glLightfv(lightId, GL_SPECULAR, specular);
		glLightfv(lightId, GL_SPOT_DIRECTION, spotDir);
		glLightfv(lightId, GL_SPOT_EXPONENT, &spotExponent);
		glLightfv(lightId, GL_SPOT_CUTOFF, &spotAngle);
	}
	else {
		glLightfv(lightId, GL_AMBIENT, ambient);
		glLightfv(lightId, GL_DIFFUSE, diffuse);
		glLightfv(lightId, GL_SPECULAR, specular);
	}

	glLightfv( lightId, GL_POSITION, position );
	glPopMatrix();

};

void PointLight::toggleLight() {
	enabled = !enabled;
};