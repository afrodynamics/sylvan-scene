#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(double, double, double, double);
	~SpotLight();
	void setCutoff(double);
	void setSpotExponent(double);
	void setSpotDir(Vector3);
};

