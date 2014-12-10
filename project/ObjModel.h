#pragma once
#include "MatrixTransform.h"
#include <string>

enum FaceDefinition {
	VERTEX_ONLY, VERTEX_SS_NORMAL, VERTEX_TEXTURE_NORMAL, QUAD
};

class ObjModel :
	public MatrixTransform
{
private:
	bool fileLoaded, printWarn;
	unsigned long faces;
	std::string filename;
	FaceDefinition faceType;

	// Parse variables, because memory?
	int symbolsRead;
	unsigned long lineNumber;
	double vx, vy, vz, vw, vtx, vty, vtz, vnx, vny, vnz;
	double r, g, b;

	// Calculate min/max and centering
	Vector3 minimum, maximum;
	double xMin, yMin, zMin, xMax, yMax, zMax;
	double windowWidth;
	
	// Material
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float shininess[4];

public:
	std::vector<Vector4> vertexList, normalList, colorList, uvwCoords;
	std::vector<int> triangleList;
	ObjModel();
	~ObjModel();
	bool parseFile(std::string fname, double);
	bool cParseFile(std::string fname);
	bool cppParseFile(std::string fname);
	bool isLoaded();
	void printInfo(std::string comment);
	void draw(Matrix4& C);
	void setMaterial(Vector4 amb, Vector4 spec, Vector4 shiny, Vector4 diff);
};

