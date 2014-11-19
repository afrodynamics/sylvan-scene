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
	windowWidth = 20.0;
}

ObjModel::~ObjModel()
{
	if (mtx != nullptr) {
		delete mtx;
	}
}

/**
 * Utility Functions (static, visible to this file only)
 *   Avoid allocating and reallocating a tokens vector every line
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

// --- end util

void ObjModel::printInfo(string comment)
{
	cout << comment << " (" << this->filename << ") " << endl
		<< "\tVertices: " << vertexList.size() << endl
		<< "\tFaces: " << faces << endl;
}

/* What the outside can use to parse a .obj file
   takes the width of the field of view at the origin */
bool ObjModel::parseFile(string fname, double winWidth) {
	windowWidth = winWidth;
	return cppParseFile(fname);
};

bool ObjModel::cParseFile(string fname) {

	/* BROKEN (but maybe faster one day) 
	 * ALSO DOESN'T COMPUTE CENTER POINT
	*/

	// Don't load a second time
	if (fileLoaded) {
		std::cerr << "Cannot load " << fname << " because this ObjModel has already loaded " << filename << endl;
		return false;
	}

	lineNumber = 0;

	/*** BEGIN PARSING ***/

	FILE *fp = fopen(fname.c_str(), "r");
	char buf[256];
	int t1, n1, t2, n2, t3, n3;
	string s1, s2, s3;
	while (fscanf(fp, "%s", buf) != 0 ) {

		if (strncmp(buf, "#", 1) == 0) {
			fscanf(fp, " %[^\n]", buf); // Discard line
			continue;
		}
		else if (strncmp(buf, "vn", 2) == 0) {
			symbolsRead = fscanf(fp, "%lf %lf %lf", &vnx, &vny, &vnz);
			if (symbolsRead == 3) {
				Vector4 norm = Vector4(vnx, vny, vnz, 0);
				norm.normalize();
				normalList.push_back(norm);
			}
		}
		else if (strncmp(buf, "v", 2) == 0) {
			symbolsRead = fscanf(fp, "%lf %lf %lf %lf %lf %lf", &vx, &vy, &vz, &r, &g, &b);
			if (symbolsRead == 6) {
				vertexList.push_back(Vector4(vx, vy, vz, 1));
				colorList.push_back(Vector4(r, g, b, 0));
			}
			else if (symbolsRead == 3) {
				vertexList.push_back(Vector4(vx, vy, vz, 1.0));
			}
		}
		else if (strncmp(buf, "f", 2) == 0) {
			fscanf(fp, "%d %[/] %d %d %[/] %d %d %[/] %d",
				&t1, &s1, &n1, &t2, &s2, &n2, &t3, &s3, &n3);
			triangleList.push_back(t1);
			triangleList.push_back(n1);
			triangleList.push_back(t2);
			triangleList.push_back(n2);
			triangleList.push_back(t3);
			triangleList.push_back(n3);
		}

	};

	fclose(fp);

	/*** END PARSING ***/

	std::cout << endl;
	filename = fname;
	fileLoaded = true;
	printInfo("Done! ");

	return true; // Successfully parsed a .obj model file
}

/**
 * Wavefront .OBJ file parser. 
 * Reference: http://en.wikipedia.org/wiki/Wavefront_.obj_file
 */
bool ObjModel::cppParseFile(string fname) {

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
	vector<string> vertexLine;
	string tok;  // first token in the string
	unsigned int fileSize = ifs.tellg();
	unsigned int filePos = 0;
	symbolsRead = 0;
	stringstream s;

	s << "Opened " << fname << "\t(" << fileSize << " bytes)" << endl;;
	cout << s.str();;

	ifs.seekg(0); // Seek to start of file

	// Prepare centering code

	xMin = yMin = zMin = DBL_MAX;
	xMax = yMax = zMax = -DBL_MAX;

	while (ifs.good()) {
		
		// Grabs a line from ifs and drops it
		// into string line
		getline(ifs, line);
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
		tokens.clear();
		tokens = split(line, ' ', tokens);
		symbolsRead = tokens.size();
		
		if (tokens.size() <= 0) {
			continue; // We're probably done reading the file
		
		}
		tok = tokens.at(0);

		/*  'f' --- Read in a face --- */
		if ( tok.compare("f") == 0 ) {
			
			/* f v1//n1 v2//n2 v3//n3 */

			vertexLine.clear();

			for (int i = 1; i < symbolsRead; ++i) {
				
				split(tokens.at(i), '/', vertexLine);
				unsigned int sz = vertexLine.size();
				unsigned int verticesPushed = 0;
				for (int j = 0; j < sz; ++j) {
					if (vertexLine.at(j).compare("") == 0) {
						continue; // An empty string between slashes can be ignored
					}
					else {
						triangleList.push_back(std::stoi(vertexLine.at(j)));
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

			if (symbolsRead == 7) {
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
			else if (symbolsRead == 5) {
				/*  v (x) (y) (z) [w] */
				vx = std::stod(tokens.at(1));
				vy = std::stod(tokens.at(2));
				vz = std::stod(tokens.at(3));
				vw = std::stod(tokens.at(4));
				vertexList.push_back(Vector4(vx, vy, vz, vw));
			}
			else if (symbolsRead == 4) {
				/*  v (x) (y) (z) */
				vx = std::stod(tokens.at(1));
				vy = std::stod(tokens.at(2));
				vz = std::stod(tokens.at(3));
				vertexList.push_back(Vector4(vx, vy, vz, 1.0));
			}

			// Check min/max

			if (vx > xMax) {
				xMax = vx;
			}
			if (vx < xMin) {
				xMin = vx;
			}

			if (vy > yMax) {
				yMax = vy;
			}
			if (vy < yMin) {
				yMin = vy;
			}

			if (vz > zMax) {
				zMax = vz;
			}
			if (vz < zMin) {
				zMin = vz;
			}

		}
		else if ( tok.compare("vn") == 0 ) { 
				
			// Normal (have only one form)

			if (symbolsRead == 4) {
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
				
			if (symbolsRead == 4) {
				/*  vt u v [w = 0] */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
				vtz = std::stod(tokens.at(3));
				//normalList.push_back(Vector4(vtx, vty, vtz, 0));
			}
			else if (symbolsRead == 3) {
				/*  vt u v */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
				//normalList.push_back(Vector4(vtx, vty, 0, 0));
			}

		}
		else if (tok.compare("vp") == 0) { 
				
			// Parametric surfaces vertices (u, [w], [v])
				
			if (symbolsRead == 4) {
				/*  vp (u) (v) (w) */
				vnx = std::stod(tokens.at(1));
				vny = std::stod(tokens.at(2));
				vnz = std::stod(tokens.at(3));
				normalList.push_back(Vector4(vnx, vny, vnz, 0));
			}
			else if (symbolsRead == 3) {
				/*  vp (u) (v) */
				vnx = std::stod(tokens.at(1));
				vny = std::stod(tokens.at(2));
				normalList.push_back(Vector4(vnx, vny, 0, 0));
			}
			else if (symbolsRead == 2) {
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

	minimum = Vector3(xMin, yMin, zMin );
	maximum = Vector3(xMax, yMax, zMax );

	// Whichever is further from the origin is our center
	Vector3 diff = maximum - minimum;

	// Now we need to interpolate between the x, y, and z to get
	// the translation we need to do
	double xMiddle = (xMin + xMax) / 2.0;
	double yMiddle = (yMin + yMax) / 2.0;
	double zMiddle = (zMin + zMax) / 2.0;

	double scaleX = windowWidth / fabs(fabs(xMax) + fabs(xMin));
	double scaleY = windowWidth / fabs(fabs(yMax) + fabs(yMin));
	double scaleZ = windowWidth / fabs(fabs(zMax) + fabs(zMin));

	double scaleFactor = fmin( fmin(scaleX, scaleY), fmin(scaleY, scaleZ) );

	Vector3 centerVector = Vector3(-xMiddle, -yMiddle, -zMiddle);	
	boundingRadius = fmax( (centerVector - minimum).length(), (centerVector - maximum).length()); 

	// Translate the model's vertices from its center to *THE* center
	Matrix4 translationMtx = Matrix4::translate(-xMiddle, -yMiddle, -zMiddle);
	mtx->transformWorld(translationMtx);

	for (int v = 0; v < vertexList.size(); ++v) {
		vertexList[v] = *mtx * vertexList[v]; // Translate vertices
	}

	// Finally, reset our matrix to the identity & scale

	mtx->identity();
	mtx->transformWorld(Matrix4::scale(scaleFactor, scaleFactor, scaleFactor));

	/** End of Centering **/
	
	cout << endl;
	filename = fname;
	fileLoaded = true;
	printInfo("Done! ");

	return true; // Successfully parsed a .obj model file
}

// Returns where this model has been laoded or not
bool ObjModel::isLoaded() { return fileLoaded; }

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
		return;
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