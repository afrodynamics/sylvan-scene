#pragma once
#include <vector>
#include "Geode.h"

class Terrain : public Geode {
private:
	std::vector<Vector3> vertices, normals;
	std::vector<Vector3> colors;
	std::vector<double> heightmap;
	int tesselX, tesselZ;
	int subdivisions;
	double maxH, minH; // Maximum and Minimum y values in heightmap

	/** Internal Methods **/
	void generateHeightmap(); // Generates the heightmap
	void generateVerts(); // Generates vertices from heightmap
	void generateNormals(); // Generates normals for each vertex
	// Generates colors for vertices based on values from heightmap
	void generateColorsFromHeightmap(Vector3 valley, Vector3 peak);

	// Vector Util
	void setHeightmap(int, int, double);
	void setColor(int, int, Vector3);
	Vector3 getColor(int, int);

	// 1D Midpoint displacement algorithm, takes line segment
	// (x1, y1, x2, y2) and initial random range & roughness const
	void midpointDisplacement(int, int, int, int, double, double, int);
	void diamondSquare(int, int, int, int, double, int, double); // rewritten to be iterative

public:
	Terrain();
	~Terrain();
	double getHeightmap(int, int);
	Vector3 getRandomGroundLocation(); // Returns a random vertex of the terrain
	void render(); // Inherited from Geode, does the drawing of the terrain
	void generate(); // Generates new terrain
  std::vector<Vector3> * getVertices();     // Return the vertices vector
};