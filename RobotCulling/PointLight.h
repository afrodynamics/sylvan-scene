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
	float spotAngle;
	int lightIndex;
public:
	PointLight();
	PointLight(double,double,double);
	~PointLight();
	void render();
	void setAmbient(float, float, float, float);
	void setDiffuse(float, float, float, float);
	void setSpecular(float, float, float, float);
	void setPosition(float, float, float, float);
};

