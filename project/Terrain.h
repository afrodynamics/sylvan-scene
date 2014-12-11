#pragma once
#include <vector>
#include "Geode.h"

class Terrain : public Geode {
private:
	std::vector<Vector3> vertices;
	std::vector<double> heightmap;
	int tesselX, tesselZ;
	int subdivisions;
public:
	Terrain();
	~Terrain();
	void render();
	void generateHeightmap();
	void generateVerts();
	double diamondSquare(int, int, int, int, double);
	double lerp(double, double, double); // Lerp between two pts
	double clamp(double, double, double); // Lerp between two pts
};