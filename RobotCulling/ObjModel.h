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
	int c1, c2;
	int symbolsRead;
	unsigned long lineNumber;
	double vx, vy, vz, vw, vtx, vty, vtz, vnx, vny, vnz;
	double r, g, b;
	int t1, t2, t3, n1, n2, n3;

public:
	std::vector<Vector4> vertexList, normalList, colorList, uvwCoords;
	std::vector<int> triangleList;
	ObjModel();
	~ObjModel();
	bool parseFile(std::string fname);
	void printInfo(std::string comment);
	void draw(Matrix4& C);
};

