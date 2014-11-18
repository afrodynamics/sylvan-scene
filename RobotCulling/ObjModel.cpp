#include "ObjModel.h"
#include <iostream>
#include <cstdlib>

using namespace std;

ObjModel::ObjModel()
{
	fileLoaded = false;
	filename = "";
	mtx = new Matrix4();
	mtx->identity();
}


ObjModel::~ObjModel()
{
	if (mtx != nullptr) {
		delete mtx;
	}
}

void ObjModel::printInfo(string comment)
{
	cout<< comment << " (" << filename << ")" << endl
		<< "\tVertices: " << vertexList.size() << endl
		<< "\tFaces: " << faceList.size() << endl;
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

	FILE *fp = nullptr;
	fp = fopen(fname.c_str(), "r");

	if (fp == nullptr) {
		cerr << "Error opening file " << fname << " for reading." << endl; return false;
	}

	// All the things we could parse (could use fewer variables, but it's for semantics)

	// Vertex Coords x, y, z, [w: default = 1]   
	// Texture coords vtx, vty, vtz (usually 0)      
	// Normals x, y, z (might not be normalized)
	double vx, vy, vz, vw, vtx, vty, vtz, vnx, vny, vnz; 
	
	// First two characters on the line
	int c1, c2;
	unsigned long lineNumber = 1;

	// Loop until we hit EOF (this is not a good way to do this, but oh well)
	while (!feof(fp)) {
		c1 = fgetc(fp); // Valid: 'v' 'f' '#'
		c2 = fgetc(fp); // Valid: ' ' 'n' 't' 
		switch (c1) {
		case 'v': /* Vertices */
			switch (c2) {
			case '#': /* Ignore Comments First */
				break;
			case ' ': // Standard Vertex (with and without w)
				if (fscanf(fp, "%lf %lf %lf %lf", &vx, &vy, &vz, &vw) == 4) {
					vertexList.push_back(Vector4(vx, vy, vz, vw));
				}
				else if (fscanf(fp, "%lf %lf %lf", &vx, &vy, &vz) == 3) {
					vertexList.push_back(Vector4(vx, vy, vz, 1.0));
				}
				break;
			case 'n': // Normal (have only one form)
				if (fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz) == 3) {
					Vector4 norm = Vector4(vnx, vny, vnz, 0);
					norm.normalize();
					normalList.push_back(norm);
				}
				break;
			case 't': // Texture Coords (u, w, [0])
				if (fscanf(fp, "%lf %lf %lf", &vtx, &vty, &vtz) == 3) {
					normalList.push_back(Vector4(vtx, vty, vtz, 0));
				}
				else if (fscanf(fp, "%lf %lf", &vtx, &vty) == 2) {
					normalList.push_back(Vector4(vtx, vty, 0, 0));
				}
				break;
			case 'p': // Parametric surfaces vertices (u, [w], [v])
				if (fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz) == 3) {
					normalList.push_back(Vector4(vnx, vny, vnz, 0));
				}
				else if (fscanf(fp, "%lf %lf", &vnx, &vny) == 2) {
					normalList.push_back(Vector4(vnx, vny, 0, 0));
				}
				else if (fscanf(fp, "%lf", &vnx) == 1) {
					normalList.push_back(Vector4(vnx, 0, 0, 0));
				}
				break;
			}
			break;
		case 'f': /* Faces */
			break;
		default:
			// Something went wrong
			cerr << "Unable to parse symbol in " << fname << " at " << lineNumber << endl;
			break;
		}
		lineNumber++; // inc. line number for error print out
	}

	fclose(fp);

	/*** END PARSING ***/

	printInfo("Loaded! ");

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