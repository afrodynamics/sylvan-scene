#pragma once
#include <vector>
#include "Geode.h"

class Terrain : public Geode {
private:
	std::vector<Vector3> vertices;
	int tesselX, tesselZ;
public:
	Terrain();
	~Terrain();
	void render();
	void generate();
	double lerp(double, double, double); // Lerp between two pts
};