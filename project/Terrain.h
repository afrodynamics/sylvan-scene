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
	void render(); // Inherited from Geode, does the drawing of the terrain
	void generateHeightmap(); // Generates the heightmap
	void generateVerts(); // Generates vertices from heightmap

	// Heightmap Util
	void setHeightmap(int,int,double);
	double getHeightmap(int,int);

	// 1D Midpoint displacement algorithm, takes line segment
	// (x1, y1, x2, y2) and initial random range & roughness const
	void midpointDisplacement(int, int, int, int, double, double, int);
	double diamondSquare(int, int, int, int, double);

	// Misc Util
	double lerp(double, double, double); // Lerp between two pts
	double clamp(double, double, double); // Lerp between two pts
};