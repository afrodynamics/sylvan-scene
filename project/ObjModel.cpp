#include "Util.h"
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
	mtx.identity();
	modelScaleMtx.identity();
	windowWidth = 20.0;
}

ObjModel::~ObjModel() {}

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
	string ft;
	switch (faceType) {
		case VERTEX_ONLY: ft = "Vertex Only"; break;
		case VERTEX_SS_NORMAL: ft = "Vertex//Normal"; break;
		case VERTEX_TEXTURE_NORMAL: ft = "Vertex/Texture/Normal"; break;
		case VERTEX_TEXTURE_ONLY: ft = "Vertex/Texture Only"; break;
		default: ft = "Uknown";
	}
	cout << comment << " (" << this->filename << ") " << endl
		<< "\tVertices: " << vertexList.size() << endl
		<< "\tTextures: " << uvwCoords.size() << endl
		<< "\tFaces: " << faces << endl
		<< "\tFaceType: " << ft << endl;
}

/* What the outside can use to parse a .obj file
   takes the width of the field of view at the origin */
bool ObjModel::parseFile(string fname, double winWidth) {
	windowWidth = winWidth;
	return cppParseFile(fname);
};

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
	bool vFound, vnFound, vtFound;
	symbolsRead = 0;
	vFound = vnFound = vtFound = false;
	stringstream s;

	s << "Opened " << fname << "\t(" << fileSize << " bytes)" << endl;
	cout << s.str();

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
			printf("  %.2f%% \t%.30s                        \r", 
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

			vFound = true; // Flag that we found vertices in the file

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
			
			vnFound = true; // Flag that we found texture coords in the file

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
			
			vtFound = true; // Flag that we found texture coords in the file

			if (symbolsRead == 4) {
				/*  vt u v [w = 0] */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
				vtz = std::stod(tokens.at(3));
                uvwCoords.push_back(Vector4(vtx, vty, vtz, 0));
				//normalList.push_back(Vector4(vtx, vty, vtz, 0));
			}
			else if (symbolsRead == 3) {
				/*  vt u v */
				vtx = std::stod(tokens.at(1));
				vty = std::stod(tokens.at(2));
                uvwCoords.push_back(Vector4(vtx, vty, 0, 0));
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

	if (ifs.bad() && ifs.fail() && !ifs.eof()) {
		std::cerr << "Parsing failed: " << strerror(errno) << endl;
		ifs.close();
		return false;
	}

	ifs.close();

	/** Set FaceType (so we know how to draw this model) **/

	if ( vFound && !vnFound && !vtFound ) {
		faceType = VERTEX_ONLY;
	}
	else if ( vFound && !vnFound && vtFound ) {
		faceType = VERTEX_TEXTURE_ONLY;
	}
	else if ( vFound && vnFound && vtFound ) {
		faceType = VERTEX_TEXTURE_NORMAL;
	}
	else if ( vFound && vnFound && !vtFound ) {
		faceType = VERTEX_SS_NORMAL;
	}

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

	double scaleX = windowWidth / Util::abs(Util::abs(xMax) + Util::abs(xMin));
	double scaleY = windowWidth / Util::abs(Util::abs(yMax) + Util::abs(yMin));
	double scaleZ = windowWidth / Util::abs(Util::abs(zMax) + Util::abs(zMin));

#ifdef __APPLE__
	double scaleFactor = std::min( std::min(scaleX, scaleY), std::min(scaleY, scaleZ) );
#else
	double scaleFactor = fmin(fmin(scaleX, scaleY), fmin(scaleY, scaleZ));
#endif
	Vector3 centerVector = Vector3(-xMiddle, -yMiddle, -zMiddle);

#ifdef __APPLE__
	boundingRadius = std::max( (centerVector - minimum).length(), (centerVector - maximum).length()); 
#else
	boundingRadius = fmax((centerVector - minimum).length(), (centerVector - maximum).length());
#endif

	// Translate the model's vertices from its center to *THE* center
	Matrix4 translationMtx = Matrix4::translate(-xMiddle, -yMiddle, -zMiddle);
	mtx.transformWorld(translationMtx);

	for (int v = 0; v < vertexList.size(); ++v) {
		vertexList[v] = mtx * vertexList[v]; // Translate vertices
	}

	// Finally, reset our matrix to the identity & scale

	Matrix4 scale = Matrix4::scale(scaleFactor, scaleFactor, scaleFactor);
	mtx.identity();
	mtx.transformWorld(scale);

	// This original matrix needs to be stored
	modelScaleMtx = mtx;
	mtx.identity(); // danger

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

	lastC = C * modelScaleMtx * mtx;
	centerPos = lastC * Vector4(0,0,0,1);
    
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR );
	glEnable(GL_COLOR_MATERIAL);

	if (!fileLoaded * !printWarn) {
		// Print a warning letting the user know what's up, use stringstreams so it's thread safe
		stringstream s;
		s << "/!\\ ObjModel Warning: \"" << filename << "\" has not been parsed yet" << endl;
		std::cout << s.str();
		printWarn = true;
		return;
	}

	// Draw *this* object, then the children

	Vector4 vtx, nrm, clr, tex;
	unsigned int normSize = normalList.size();
	unsigned int vertSize = vertexList.size();
    unsigned int uvwSize = uvwCoords.size();
	unsigned int colorSize = vertexList.size();

	Matrix4 tmp = lastC;
	tmp.transpose();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(tmp.getPointer());

	glBegin(GL_TRIANGLES);

	bool lookupColor = false;
	// glColor3f(0.0, 0.0, 0.0); // Default to color black
	for (int i = 0; i < triangleList.size(); ) {

		// Grab the normal and vertex indices in their respective arrays
		// Only grab the values from the array and inc i if those values
		// should be pulled (see facetype definitions)
		
		int vtxIndex = -1, nrmIndex = -1, uvwIndex = -1;

		if ( faceType == VERTEX_ONLY || faceType == VERTEX_SS_NORMAL || faceType == VERTEX_TEXTURE_NORMAL || faceType == VERTEX_TEXTURE_ONLY ) { 
			vtxIndex = triangleList[i++] - 1; 
		}

		if ( faceType == VERTEX_ONLY || faceType == VERTEX_SS_NORMAL || faceType == VERTEX_TEXTURE_NORMAL ) { 
			nrmIndex = triangleList[i++] - 1; 
		} 

        if ( faceType == VERTEX_ONLY || faceType == VERTEX_TEXTURE_NORMAL || faceType == VERTEX_TEXTURE_ONLY ) { 
        	uvwIndex = triangleList[i++] - 1; 
        } 

		// if (vtxIndex >= vertSize || uvwIndex >= uvwSize || nrmIndex >= normSize) break;

		if (vtxIndex >= colorSize) {
			lookupColor = false;
		}
		else if ( lookupColor && vtxIndex > 0 ) {
			clr = colorList[vtxIndex];
			glColor3f(clr.getX(), clr.getY(), clr.getZ());
		}

		// Grab the information we need from our std::vectors (in Vector4 format)
		/* Vertices Only */
		if (faceType == VERTEX_ONLY) {
			vtx = vertexList[vtxIndex];
            glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());
		}
		/* Vertex & Normal Pairs */
		else if (faceType == VERTEX_SS_NORMAL) {
			vtx = vertexList[vtxIndex];
			nrm = normalList[nrmIndex];
            glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
            glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());
		}
		/* Vertex & Texture Pairs */
		else if (faceType == VERTEX_TEXTURE_ONLY) {
			vtx = vertexList[vtxIndex];
            tex = uvwCoords[uvwIndex];
            glTexCoord3f(tex.getX(), tex.getY(), tex.getZ());
			glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());
		}
		/* Vertex, Textures & Normals */
		else if (faceType == VERTEX_TEXTURE_NORMAL) {
			vtx = vertexList[vtxIndex];
            tex = uvwCoords[uvwIndex];
            nrm = normalList[nrmIndex];
            glNormal3f(nrm.getX(), nrm.getY(), nrm.getZ());
	        glTexCoord3f(tex.getX(), tex.getY(), tex.getZ());
			glVertex3f(vtx.getX(), vtx.getY(), vtx.getZ());
		}
		
	}

	glEnd();
	
	// Call draw on the children, if we have any
	Group::draw(lastC);

}

// TODO: stip this function & replace with the Material class
void ObjModel::setMaterial(Vector4 amb, Vector4 spec, Vector4 shiny, Vector4 diff) {
	ambient[0] = amb.getX();
	ambient[1] = amb.getY();
	ambient[2] = amb.getZ();
	ambient[3] = amb.getW();
	
	shininess[0] = shiny.getX();
	shininess[1] = shiny.getY();
	shininess[2] = shiny.getZ();
	shininess[3] = shiny.getW();

	specular[0] = spec.getX();
	specular[1] = spec.getY();
	specular[2] = spec.getZ();
	specular[3] = spec.getW();

	diffuse[0] = diff.getX();
	diffuse[1] = diff.getY();
	diffuse[2] = diff.getZ();
	diffuse[3] = diff.getW();
}