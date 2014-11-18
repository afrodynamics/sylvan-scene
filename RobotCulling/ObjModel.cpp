#include "ObjModel.h"
#include <iostream>
#include <cstdlib>

using namespace std;

ObjModel::ObjModel()
{
	fileLoaded = false;
	filename = "";
	faces = 0;
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
	cout << comment << " (" << this->filename << ") " << endl
		<< "\tVertices: " << vertexList.size() << endl
		<< "\tTriangles: " << faces << endl;
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
	double r, g, b;

	// First two characters on the line
	int c1, c2;
	int symbolsRead = 0;
	unsigned long lineNumber = 1;

	// Support a LOT of vertices
	unsigned long t1, t2, t3, n1, n2, n3;

	// Loop until we hit EOF (this is not a good way to do this, but oh well)
	while (!feof(fp)) {
		symbolsRead = 0;
		c1 = fgetc(fp); // Valid: 'v' 'f' '#'
		c2 = fgetc(fp); // Valid: ' ' 'n' 't' 

		if (c1 == 'f') {
			symbolsRead = fscanf(fp, "%d//%d %d//%d %d//%d",
				&t1, &n1, &t2, &n2, &t3, &n3); // Vertex/Normal

			// Push a triangle corner index, then
			triangleList.push_back(t1);
			triangleList.push_back(n1);
			triangleList.push_back(t2);
			triangleList.push_back(n2);
			triangleList.push_back(t3);
			triangleList.push_back(n3);

			faces++; // Inc face counter
		}
		else if (c1 == 'v') {
			switch (c2) {
			case ' ': // Standard Vertex (with and without w)
				symbolsRead = fscanf(fp, "%lf %lf %lf %lf %lf %lf", &vx, &vy, &vz, &r, &g, &b);
				if (symbolsRead == 6) {
					vertexList.push_back(Vector4(vx, vy, vz, 1));
					colorList.push_back(Vector4(r, g, b, 0));
				}
				else if (symbolsRead == 3) {
					vertexList.push_back(Vector4(vx, vy, vz, 1.0));
				}
				break;
			case 'n': // Normal (have only one form)
				symbolsRead = fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz);
				if (symbolsRead == 3) {
					Vector4 norm = Vector4(vnx, vny, vnz, 0);
					norm.normalize();
					normalList.push_back(norm);
				}
				break;
			case 't': // Texture Coords (u, w, [0])
				symbolsRead = fscanf(fp, "%lf %lf %lf", &vtx, &vty, &vtz);
				if (symbolsRead == 3) {
					normalList.push_back(Vector4(vtx, vty, vtz, 0));
				}
				else if (symbolsRead == 2) {
					normalList.push_back(Vector4(vtx, vty, 0, 0));
				}
				break;
			case 'p': // Parametric surfaces vertices (u, [w], [v])
				symbolsRead = fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz);
				if (symbolsRead == 3) {
					normalList.push_back(Vector4(vnx, vny, vnz, 0));
				}
				else if (symbolsRead == 2) {
					normalList.push_back(Vector4(vnx, vny, 0, 0));
				}
				else if (symbolsRead == 1) {
					normalList.push_back(Vector4(vnx, 0, 0, 0));
				}
				break;
			}
		}

		lineNumber++;
		
	}

	fclose(fp);

	/*** END PARSING ***/

	cerr << "Read " << lineNumber << " lines" << endl;
	printInfo("Loaded! ");
	fileLoaded = true;

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

	Vector4 vtx, nrm, clr;
	unsigned int normSize = normalList.size();
	unsigned int vertSize = vertexList.size();
	unsigned int colorSize = vertexList.size();

	Matrix4 tmp = lastC;
	tmp.transpose();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(tmp.getPointer());
	glBegin(GL_TRIANGLES);

	for ( auto iter = triangleList.begin(); iter != triangleList.end(); ) {

		// Grab the normal and vertex indices in their respective arrays

		unsigned int corner1 = (*iter); ++iter;
		unsigned int normal1 = (*iter); ++iter;
		unsigned int corner2 = (*iter); ++iter;
		unsigned int normal2 = (*iter); ++iter;
		unsigned int corner3 = (*iter); ++iter;
		unsigned int normal3 = (*iter); ++iter;
		bool lookupColor = false;

		if (corner1 >= vertSize || corner2 >= vertSize || corner3 >= vertSize ) break;
		if (normal1 >= normSize || normal2 >= normSize || normal3 >= normSize) break;
		if (corner1 >= colorSize || corner2 >= colorSize || corner3 >= colorSize) {
			glColor3f(1.0, 1.0, 1.0); // Default to color white
		}
		else {
			lookupColor = true;
		}

		// Grab the information we need from our std::vectors (in Vector4 format)

		vtx = vertexList[corner1];
		nrm = normalList[normal1];
		clr;
		if (lookupColor) {
			clr = colorList[corner1];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		/* First Corner */
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());

		vtx = vertexList[corner2];
		nrm = normalList[normal2];
		clr;
		if (lookupColor) {
			clr = colorList[corner2];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		/* Second Corner */
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());

		vtx = vertexList[corner3];
		nrm = normalList[normal3];
		clr;
		if (lookupColor) {
			clr = colorList[corner3];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		/* Third Corner */
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());

	}

	glEnd();

	// Call draw on the children, if we have any
	Group::draw(lastC);

}