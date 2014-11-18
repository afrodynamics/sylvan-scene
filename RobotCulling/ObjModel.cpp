#include "ObjModel.h"
#include <iostream>

using namespace std;

ObjModel::ObjModel()
{
	fileLoaded = false;
	filename = "";
}


ObjModel::~ObjModel()
{
	if (mtx != nullptr) {
		delete mtx;
	}
}

void ObjModel::printInfo(string comment)
{
	cout<< comment << " " << endl
		<< "Vertices: " << vertexList.size() << endl
		<< "Faces: " << faceList.size() << endl;
}

/**
 * Wavefront .OBJ file parser. 
 * Reference: http://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
bool ObjModel::parseFile(string fname) {

	// Don't load a second time
	if (fileLoaded) {
		cerr << "Cannot load " << fname << " because this ObjModel has already loaded " << filename << endl;
		return false;
	}

	/*** BEGIN PARSING ***/



	/*** END PARSING ***/

	return true; // Successfully parsed a .obj model file
}

/**
 * Draws the model if it has been loaded into memory
 */
void ObjModel::draw(Matrix4& C) {

	lastC = C * *mtx;
	centerPos = lastC * Vector4(0,0,0,1);

	if (!fileLoaded) {
		// Print a warning letting the user know what's up
		cerr << "Warning, attempting to draw an ObjModel which has not yet been parsed..." << endl;
	}

	// Draw *this* object, then the children

	glBegin(GL_TRIANGLES);
	glEnd();

	// Call draw on the children, if we have any
	Group::draw(lastC);

}