#pragma once
#include "MatrixTransform.h"
#include <string>

class ObjModel :
	public MatrixTransform
{
private:
	bool fileLoaded;
	unsigned long faces;
	std::string filename;
public:
	std::vector<Vector4> vertexList, normalList, colorList, uvwCoords;
	std::vector<int> triangleList;
	ObjModel();
	~ObjModel();
	bool parseFile(std::string fname);
	void printInfo(std::string comment);
	void draw(Matrix4& C);
};

