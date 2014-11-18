#include "ObjModel.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

ObjModel::ObjModel()
{
	fileLoaded = printWarn = false;
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

	lineNumber = 0;

	/*** BEGIN PARSING ***/

	ifstream ifs(fname, ios::in | ios::ate);
	string line;
	vector<string> tokens;
	string tok;  // first token in the string
	unsigned int fileSize = ifs.tellg();
	unsigned int filePos = 0;
	stringstream s;
	s << "Opened " << fname << " (" << fileSize << " bytes)" << endl;;

	cout << s.str();;

	ifs.seekg(0);

	while (ifs.good()) {
		
		// Grabs a line from ifs and drops it
		// into string line
		getline(ifs, line, '\n');
		lineNumber++; 
		
		// Don't waste time splitting the string if we don't have to

		/*  '#' --- Skip Comments --- */
		if ( line.size() > 0 && line.at(0) == '#' ) {
			continue;
		}

		filePos = ifs.tellg();

		// So the TAs know stuff is actually going on and it isn't hanging
		if (lineNumber % 4096 == 0) {
			float progress = (float)filePos / (float)fileSize;
			printf("  %.2f%% \t%.16s                        \r", 
				   progress * 100, fname.c_str() );
			fflush(stdout);
		}

		// OBJ files are space-delimited
		tokens = split(line, ' ');
		
		if (tokens.size() <= 0) {
			continue; // We're probably done reading the file
		
		}
		tok = tokens.at(0);

		/*  'f' --- Read in a face --- */
		if ( tok.compare("f") == 0 ) {
			
			/* f v1//n1 v2//n2 v3//n3 */

			for (int i = 1; i < tokens.size(); ++i) {
				
				vector<string> v = split(tokens.at(i), '/');
				unsigned int sz = v.size();
				unsigned int verticesPushed = 0;
				for (int j = 0; j < sz; ++j) {
					if ( v.at(j).compare("") == 0) {
						continue; // An empty string between slashes can be ignored
					}
					else {
						triangleList.push_back( std::stoi( v.at(j) ) );
						verticesPushed++;
					}
					
				}
				if (verticesPushed == 1) faceType = VERTEX_ONLY;
				else if (verticesPushed == 2) faceType = VERTEX_SS_NORMAL;
				else if (verticesPushed == 3) faceType = VERTEX_TEXTURE_NORMAL;
				
			}

			faces++; // Inc face counter
		}
		/*  'v' --- Read in a vertex --- */
		else if (tok.compare("v") == 0) {

			/* This is a vertex line */

			if (tokens.size() == 7) {
				/*  v (x) (y) (z) (r) (g) (b)  */
				vx = std::stod( tokens.at(1) );
				vy = std::stod(tokens.at(2));
				vz = std::stod(tokens.at(3));
				r = std::stod(tokens.at(4));
				g = std::stod(tokens.at(5));
				b = std::stod(tokens.at(6));
				vertexList.push_back(Vector4(vx, vy, vz, 1));
				colorList.push_back(Vector4(r, g, b, 0));
			}
			else if (tokens.size() == 5) {
				/*  v (x) (y) (z) [w] */
				vx = std::stod(tokens.at(1));
				vy = std::stod(tokens.at(2));
				vz = std::stod(tokens.at(3));
				vw = std::stod(tokens.at(4));
				vertexList.push_back(Vector4(vx, vy, vz, vw));
			}
			else if (tokens.size() == 4) {
				/*  v (x) (y) (z) */
				vx = std::stod(tokens.at(1));
				vy = std::stod(tokens.at(2));
				vz = std::stod(tokens.at(3));
				vertexList.push_back(Vector4(vx, vy, vz, 1.0));
			}

		}
		else if ( tok.compare("vn") == 0 ) { 
				
			// Normal (have only one form)

			if (tokens.size() == 4) {
				/*  vn (x) (y) (z) */
				vnx = std::stod(tokens.at(1));
				vny = std::stod(tokens.at(2));
				vnz = std::stod(tokens.at(3));
				Vector4 norm = Vector4(vnx, vny, vnz, 0);
				norm.normalize();
				normalList.push_back(norm);
			}

		}
		else if (tok.compare("vt") == 0) { 
				
			// Texture Coords (u, w, [0])
				
			if (tokens.size() == 4) {
				/*  vt u v [w = 0] */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
				vtz = std::stod(tokens.at(3));
				//normalList.push_back(Vector4(vtx, vty, vtz, 0));
			}
			else if (tokens.size() == 3) {
				/*  vt u v */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
				//normalList.push_back(Vector4(vtx, vty, 0, 0));
			}

		}
		else if (tok.compare("vp") == 0) { 
				
			// Parametric surfaces vertices (u, [w], [v])
				
			if (tokens.size() == 4) {
				/*  vp (u) (v) (w) */
				vnx = std::stod(tokens.at(1));
				vny = std::stod(tokens.at(2));
				vnz = std::stod(tokens.at(3));
				normalList.push_back(Vector4(vnx, vny, vnz, 0));
			}
			else if (tokens.size() == 3) {
				/*  vp (u) (v) */
				vnx = std::stod(tokens.at(1));
				vny = std::stod(tokens.at(2));
				normalList.push_back(Vector4(vnx, vny, 0, 0));
			}
			else if (tokens.size() == 2) {
				/*  vp (u) */
				vnx = std::stod(tokens.at(1));
				normalList.push_back(Vector4(vnx, 0, 0, 0));
			}
		}
	}

	if (ifs.fail()) {
		std::cerr << "Parsing failed: " << strerror(errno) << endl;
		ifs.close();
		return false;
	}

	ifs.close();

	/*** END PARSING ***/

	cout << endl;
	filename = fname;
	fileLoaded = true;
	printInfo("Done! ");

	return true; // Successfully parsed a .obj model file
}

/**
 * Draws the model if it has been loaded into memory
 */
void ObjModel::draw(Matrix4& C) {

	lastC = C * *mtx;
	centerPos = lastC * Vector4(0,0,0,1);

	if (!fileLoaded * !printWarn) {
		// Print a warning letting the user know what's up, use stringstreams so it's thread safe
		stringstream s;
		s << "/!\\ ObjModel Warning: \"" << filename << "\" has not been parsed yet" << endl;
		std::cout << s.str();
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

	bool lookupColor = false;

	for (int i = 0; i < triangleList.size(); ) {

		// Grab the normal and vertex indices in their respective arrays
		unsigned int vtxIndex = triangleList[i++] - 1;
		unsigned int nrmIndex = triangleList[i++] - 1;

		if (vtxIndex >= vertSize) break;
		if (nrmIndex >= normSize) break;
		if (vtxIndex >= colorSize) {
			glColor3f(1.0, 1.0, 1.0); // Default to color white
			lookupColor = false;
		}
		else if (lookupColor) {
			clr = colorList[vtxIndex];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		// Grab the information we need from our std::vectors (in Vector4 format)
		/* Vertex 1 */
		
		if (faceType == VERTEX_SS_NORMAL) {
			vtx = vertexList[vtxIndex];
			nrm = normalList[nrmIndex];
			clr;
		}
		else {
			break; // Unsupported face type right now
		}
				
		glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
		glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());	

	}

	glEnd();

	// Call draw on the children, if we have any
	Group::draw(lastC);

}