#include "SpotLight.h"
#include "Wiresphere.h"

SpotLight::SpotLight()
{
	positionSphere = new Wiresphere();
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
	positionSphere = new Wiresphere();
	centerPos = Vector4(x, y, z, 1);
	setPosition(x, y, z, 1);
	setAmbient(0, 0, 0, 1);
	setDiffuse(1, 1, 1, 1);
	setSpecular(1, 1, 1, 1);
	setCutoff(angle);

	lightIndex = Node::lightCounter;
	Node::lightCounter++;
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