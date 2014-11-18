#include "ObjModel.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

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

/**
 * Utility Functions (static, visible to this file only)
 */
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
// --- end util

void ObjModel::printInfo(string comment)
{
	cout << comment << " (" << this->filename << ") " << endl
		<< "\tVertices: " << vertexList.size() << endl
		<< "\tFaces: " << faces << endl;
}

/**
 * Wavefront .OBJ file parser. 
 * Reference: http://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
bool ObjModel::parseFile(string fname) {

	// Don't load a second time
	if (fileLoaded) {
		std::cerr << "Cannot load " << fname << " because this ObjModel has already loaded " << filename << endl;
		return false;
	}

	// All the things we could parse (could use fewer variables, but it's for semantics)

	// Vertex Coords x, y, z, [w: default = 1]   
	// Texture coords vtx, vty, vtz (usually 0)      
	// Normals x, y, z (might not be normalized)
	/*double vx, vy, vz, vw, vtx, vty, vtz, vnx, vny, vnz;
	double r, g, b;
	*/
	symbolsRead = 0;
	lineNumber = 1;

	// Support a LOT of vertices
	//int t1, t2, t3, n1, n2, n3;

	/*** BEGIN PARSING ***/

	ifstream ifs;
	string line;
	vector<string> tokens;
	string tok;  // first token in the string

	ifs.open(fname); /// NO MATTER WHAT I DO IT WON'T WORK
	

	while (ifs.good()) {
		
		// Grabs a line from ifs and drops it
		// into string line
		getline(ifs, line, '\n');

		// OBJ files are space-delimited
		tokens = split(line, ' ');	

		if (tokens.size() <= 0) {
			continue; // We're probably done reading the file
		}

		tok = tokens.at(0).front();

		/*  '#' --- Skip Comments --- */
		if ( tok.compare("#") == 0) {
			lineNumber++; continue;
		}
		/*  'f' --- Read in a face --- */
		if ( tok.compare("f") == 0 ) {
			
			// Push a triangle corner index, then
			triangleList.push_back(t1);
			triangleList.push_back(n1);
			triangleList.push_back(t2);
			triangleList.push_back(n2);
			triangleList.push_back(t3);
			triangleList.push_back(n3);

			faces++; // Inc face counter
		}
		/*  'v' --- Read in a vertex --- */
		else if (tok.compare("v") == 0) {

			/* This is a vertex line */

			if (tokens.size() == 6) {
				vertexList.push_back(Vector4(vx, vy, vz, 1));
				colorList.push_back(Vector4(r, g, b, 0));
			}
			else if (tokens.size() == 3) {
				vertexList.push_back(Vector4(vx, vy, vz, 1.0));
			}

		}
		else if ( tok.compare("vn") ) { 
				
			// Normal (have only one form)

			if (tokens.size() == 3) {
				Vector4 norm = Vector4(vnx, vny, vnz, 0);
				norm.normalize();
				normalList.push_back(norm);
			}

		}
		else if (tok.compare("vt")) { 
				
			// Texture Coords (u, w, [0])
				
			if (tokens.size() == 3) {
				normalList.push_back(Vector4(vtx, vty, vtz, 0));
			}
			else if (tokens.size() == 2) {
				normalList.push_back(Vector4(vtx, vty, 0, 0));
			}

		}
		else if (tok.compare("vp")) { 
				
			// Parametric surfaces vertices (u, [w], [v])
				
			if (tokens.size() == 3) {
				normalList.push_back(Vector4(vnx, vny, vnz, 0));
			}
			else if (tokens.size() == 2) {
				normalList.push_back(Vector4(vnx, vny, 0, 0));
			}
			else if (symbolsRead == 1) {
				normalList.push_back(Vector4(vnx, 0, 0, 0));
			}
		}

		lineNumber++;

	}

	if (ifs.fail()) {
		std::cerr << "Parsing failed: " << strerror(errno) << endl;
		ifs.close();
		return false;
	}

	ifs.close();

//	FILE *fp = nullptr;
//	fp = fopen(fname.c_str(), "r");

/*	if (fp == nullptr) {
		cerr << "Error opening file " << fname << " for reading." << endl; return false;
	}

	// Loop until we hit EOF (this is not a good way to do this, but oh well)
	while (!feof(fp)) {
		
		symbolsRead = 0;
		c1 = fgetc(fp); // Valid: 'v' 'f' '#'
		c2 = fgetc(fp); // Valid: ' ' 'n' 't' 

		if (c1 == 'f') {
			symbolsRead = fscanf(fp, "%d %d %d %d %d %d",
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
			if (c2 == ' ') { // Standard Vertex (with and without w)
				symbolsRead = fscanf(fp, "%lf %lf %lf %lf %lf %lf", &vx, &vy, &vz, &r, &g, &b);
				if (symbolsRead == 6) {
					vertexList.push_back(Vector4(vx, vy, vz, 1));
					colorList.push_back(Vector4(r, g, b, 0));
				}
				else if (symbolsRead == 3) {
					vertexList.push_back(Vector4(vx, vy, vz, 1.0));
				}
			}
			else if (c2 == 'n') { // Normal (have only one form)
				symbolsRead = fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz);
				if (symbolsRead == 3) {
					Vector4 norm = Vector4(vnx, vny, vnz, 0);
					norm.normalize();
					normalList.push_back(norm);
				}
			}
			else if (c2 == 't') { // Texture Coords (u, w, [0])
				symbolsRead = fscanf(fp, "%lf %lf %lf", &vtx, &vty, &vtz);
				if (symbolsRead == 3) {
					normalList.push_back(Vector4(vtx, vty, vtz, 0));
				}
				else if (symbolsRead == 2) {
					normalList.push_back(Vector4(vtx, vty, 0, 0));
				}
			}
			else if (c2 == 'p') { // Parametric surfaces vertices (u, [w], [v])
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
			}
		}

		lineNumber++;
		
	}

	fclose(fp); */

	/*** END PARSING ***/

	std::cerr << "Read " << lineNumber << " lines" << endl;
	filename = fname;
	fileLoaded = true;
	printInfo("Loaded! ");

	return true; // Successfully parsed a .obj model file
}

/**
 * Draws the model if it has been loaded into memory
 */
void ObjModel::draw(Matrix4& C) {

	static bool printWarn = false;
	lastC = C * *mtx;
	centerPos = lastC * Vector4(0,0,0,1);

	if (!fileLoaded * !printWarn) {
		// Print a warning letting the user know what's up
		std::cerr << "Warning, attempting to draw an ObjModel which has not yet been parsed..." << endl;
		printWarn = true;
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

	for (int i = 1; i <= faces; ++i) {

		// Grab the normal and vertex indices in their respective arrays

		bool lookupColor = false;

		if (i + 2 >= vertSize) break;
		if (i + 2 >= normSize) break;
		if (i + 2 >= colorSize) {
			glColor3f(1.0, 1.0, 1.0); // Default to color white
		}
		else {
			lookupColor = true;
		}

		// Grab the information we need from our std::vectors (in Vector4 format)
		/* Vertex 1 */
		vtx = vertexList[i-1];
		nrm = normalList[i-1];
		clr;
		if (lookupColor) {
			clr = colorList[i-1];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());	
		
		/* Vertex 2 */
		++i;
		vtx = vertexList[i - 1];
		nrm = normalList[i - 1];
		clr;
		if (lookupColor) {
			clr = colorList[i - 1];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}
		
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());

		/* Vertex 3 */
		++i;
		vtx = vertexList[i - 1];
		nrm = normalList[i - 1];
		clr;
		if (lookupColor) {
			clr = colorList[i - 1];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());

	}

	glEnd();

	// Call draw on the children, if we have any
	Group::draw(lastC);

}