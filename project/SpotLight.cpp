#include "SpotLight.h"
#include "Cone.h"

SpotLight::SpotLight()
{
	positionSphere = new Cone();
	lightIndex = Node::lightCounter;

	// Set light properties to OpenGL default

	setPosition(0, 0, 0, 1);
	setAmbient(0, 0, 0, 1);
	setDiffuse(1, 1, 1, 1);
	setSpecular(1, 1, 1, 1);
	setCutoff(45);

	Node::lightCounter++;
}

/**
* Point lights are always in world coordinates
* because in PointLight::draw() we pop the GL
* state and load the identity
*/
SpotLight::SpotLight(double x, double y, double z, double angle)
{
	positionSphere = new Cone();
	centerPos = Vector4(x, y, z, 1);
	setPosition(x, y, z, 1);
	setAmbient(0, 0, 0, 1);
	setDiffuse(1, 1, 1, 1);
	setSpecular(1, 1, 1, 1);
	setCutoff(angle);
	setSpotExponent(2.0);
	setSpotDir(Vector3(0,0,-1.0));

	lightIndex = Node::lightCounter;
	Node::lightCounter++;
	enabled = true;
}

SpotLight::~SpotLight()
{
	delete positionSphere;
	Node::lightCounter--;
}

void SpotLight::setCutoff(double d) {
	if (d > 90 || d < 0) return; // Invalid values
	spotAngle = d;
}

void SpotLight::setSpotExponent(double e) {
	spotExponent = e;
}

void SpotLight::setSpotDir(Vector3 v) {
	spotDir[0] = v.getX();
	spotDir[1] = v.getY();
	spotDir[2] = v.getZ();
}