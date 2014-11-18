#pragma once
#include "MatrixTransform.h"
#include <string>

class ObjModel :
	public MatrixTransform
{
private:
	bool fileLoaded;
	std::string filename;
public:
	std::vector<Vector4> faceList, vertexList, normalList, colorList, uvwCoords;
	ObjModel();
	~ObjModel();
	bool parseFile(std::string fname);
	void printInfo(std::string comment);
	void draw(Matrix4& C);
};

