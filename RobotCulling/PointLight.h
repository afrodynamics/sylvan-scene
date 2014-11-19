#pragma once
#include "Geode.h"
class PointLight :
	public Geode
{
protected:
	Geode *positionSphere;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
	float spotDir[3];
	float spotAngle;
	float spotExponent;
	int lightIndex;
	bool enabled;
public:
	PointLight();
	PointLight(double,double,double);
	~PointLight();
	void render();
	void setAmbient(float, float, float, float);
	void setDiffuse(float, float, float, float);
	void setSpecular(float, float, float, float);
	void setPosition(float, float, float, float);
	void toggleLight();
};

