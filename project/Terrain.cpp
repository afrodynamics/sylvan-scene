#include <iostream>
#include "Util.h"
#include "Terrain.h"

using namespace std;
using namespace Util;

Terrain::Terrain() {

	// Seed the RNG
	Util::seed(0);

	// Prepare the resolution of the heightmap
	subdivisions = 8; // Be careful, exponential time going on here!
	tesselX = tesselZ = pow(2, subdivisions) + 1; // This will be important later

	// And then, in an instant, a new world was born
	generate();
};

Terrain::~Terrain() {
	// Nothing to destruct
}

/**
 * Calls all the subroutines needed to generate new terrain
 */
void Terrain::generate() {
	// Reset min/max height
	minH = 1.0;
	maxH = 0.0;
	generateHeightmap();
	generateVerts(); // Generate vertices for the terrain
	generateNormals();
	//generateColorsFromHeightmap(Vector3(0.25, 0.9, 0.0), Vector3(.95, .95, .95)); // trying to look like earth
	generateColorsFromHeightmap(Vector3(0.85, 0.85, 0.85), Vector3(1.0, 1.0, 1.0)); // white - gray
	//generateColorsFromHeightmap(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0)); // grayscale
}

/**
 * Returns a random vertex from the heightmap
 */
Vector3 Terrain::getRandomGroundLocation() {
	int x = floor(drandRange(0, tesselX - 1));
	int z = floor(drandRange(0, tesselZ - 1));
	return Vector3(
		(double)x / (double)tesselX, getHeightmap(x, z), (double)z / (double)tesselZ
	);
}

/**
 * Generates the vertices of the terrain, filling in the y coordinate
 * with the value in the heightmap. Call this ***after*** generateHeightmap()
 */
void Terrain::generateVerts() {
	// Definitions
	double minX, minZ;
	double maxX, maxZ;
	minX = minZ = -1.0;
	maxX = maxZ = 1.0;

	// If we're calling this function again
	vertices.clear();

	// Fill the vertices array
	for (int x = 0; x < tesselX; ++x) {
		for (int z = 0; z < tesselZ; ++z) {
			double tx = (double)x / (double)tesselX;
			double tz = (double)z / (double)tesselZ;
			vertices.push_back(Vector3(
				lerp(tx, minX, maxX),
				getHeightmap(x, z),
				lerp(tz, minZ, maxZ))
			);
		}
	}
}

/**
 * Generates a heightmap by populating a square 2D array.
 * Heightmap is in ROW-MAJOR, so the pixel (X,Y) is
 *     heightmap[ Y * tesselX + X ]
 */
void Terrain::generateHeightmap() {

	// Make sure we have enough memory
	if (heightmap.size() < tesselX * tesselZ) {
		heightmap.resize( tesselX * tesselZ );
	}
		
	for ( int i = 0; i < tesselX; ++i ) {
		for ( int j = 0; j < tesselZ; ++j ) {
			setHeightmap(i,j,0.0);
		}
	}

	// Four corners should be set to something random though
	// setHeightmap(0, 0, drandRange(0.0, 0.5));
	// setHeightmap(0, tesselZ - 1, drandRange(-.125, 0.125));
	// setHeightmap(tesselX - 1, 0, drandRange(-.125, 0.125));
	// setHeightmap(tesselX - 1, tesselZ - 1, drandRange(0.0, 0.5));
	
	double roughness = 1.0;//drandRange(.5,1.0);
	double random = drandRange(.25,1.0); // default .0625
	diamondSquare(0,0,tesselX - 1, tesselZ - 1, random, tesselX - 1, roughness);

	// Make sure everything is averaged out

};

// Generates vertex colors for each vertex based on the height of
// the given vertex in the heightmap. Takes two Vector3s which are
// the RGB values (respectively) of the low altitude & high altitude
// vertices. (Essentially the endpoints of a lerp.)
void Terrain::generateColorsFromHeightmap(Vector3 valley, Vector3 peak) {
	// Definitions
	double minX, minZ;
	double maxX, maxZ;
	minX = minZ = -1.0;
	maxX = maxZ = 1.0;

	// If we're calling this function again
	colors.clear();

	// Fill the colors array
	for (int x = 0; x < tesselX; ++x) {
		for (int z = 0; z < tesselZ; ++z) {
			double tx = (double)x / (double)tesselX;
			double tz = (double)z / (double)tesselZ;
			double t = Util::abs(getHeightmap(x, z) / (maxH - minH) ); // We should normalize heightmaps
			if (getHeightmap(x, z) > 0) {
				colors.push_back(lerp(t, valley, peak));
			}
			else {
				colors.push_back(valley); // If it's too low or negative, just use valley color
			}
			//colors.push_back( normals[ z * tesselX + x ] ); // use colors for normals
		}
	}
}

/**
 * Generates the normals for each vertex in the terrain. 
 * Call this *** after *** generateVerts()
 */
void Terrain::generateNormals() {

	// Definitions
	Vector3 p0, p1, p2, p3, n0, n1, n2, n3, xVec, yVec;

	// If we're calling this function again
	normals.clear();
	normals.resize(tesselX * tesselZ);

	// Fill the normals array
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			
			p0 = vertices.at( j * tesselX + i );
			p1 = vertices.at( j * tesselX + i + 1 );
			p2 = vertices.at( (j+1) * tesselX + i );
			p3 = vertices.at( (j+1) * tesselX + (i+1) );

			xVec = p1 - p0;
			yVec = p2 - p0;
			n0 = xVec.cross(xVec,yVec);
			n0.normalize();

			normals[ j * tesselX + i ] = n0;
			normals[ j * tesselX + i + 1 ] = n0;
			normals[ (j+1) * tesselX + i ] = n0;
			normals[ (j+1) * tesselX + i + 1 ] = n0;

		}
	}
}

// Getters and setters for the various vectors
void Terrain::setHeightmap(int x, int y, double d) {
   heightmap[ y * tesselZ + x ] = d;
};

double Terrain::getHeightmap(int x,int y) {
	return heightmap.at(y * tesselZ + x);
};

void Terrain::setColor(int x, int y, Vector3 d) {
	colors[y * tesselZ + x] = d;
};

Vector3 Terrain::getColor(int x, int y) {
	return colors.at(y * tesselZ + x);
};

// Midpoint Displacement algorithm
// takes line segment (x1, y1, x2, y2) and initial random range
void Terrain::midpointDisplacement(int x1, int y1, int x2, int y2, double randRange, double rough, int iterations ) {
	int xDist = x2 - x1;
	int yDist = y2 - y1;
	int xMid = xDist/2;
	int yMid = yDist/2;
	double avgHeight = lerp( 0.5, getHeightmap(x1, y1), getHeightmap(x2, y2) );
	double h = drandRange(-randRange,randRange) + avgHeight;
	rough = clamp(rough, 0, 1.0);
	setHeightmap(xMid, yMid, h);
	iterations--;
	if ( iterations > 0 ) {
		midpointDisplacement( x1, y1, xMid, yMid, randRange * rough, rough, iterations );
		midpointDisplacement( xMid, yMid, x2, y2, randRange * rough, rough, iterations );
	}
};

/**
 * An implementation of the diamond-square terrain generation algorithm.
 * Described in great detail here: http://www.gameprogrammer.com/fractal.html
 * A 2D application of the midpoint displacement algorithm.
 *
 * Originally I implemented this recursively, but some striations were visible in the terrain.
 * Reimplemented iteratively, with help from the internet.
 *
 * I referred to some other sources to verify my understanding of the algorithm, and found
 * some help with the iterative rewrite when things stopped working:
 * http://www.paulboxley.com/blog/2011/03/terrain-generation-mark-one
 * http://gamedev.stackexchange.com/questions/37389/diamond-square-terrain-generation-problem
 *
 * (x1,y1) and (x2,y2) represent the top left and bottom right corners of the heightmap,
 * range is the maximum random displacement, and level is the depth of the recursion.
 * roughness is another constant 0.0 <= x <= 1.0 which changes the terrain's random range.
 * We left this at 1 for now, since it seemed to work well with our scene.
 */
void Terrain::diamondSquare(int x1, int y1, int x2, int y2, double range, int level, double roughness ) {
	// If we'd be recursing on a sub-pixel-by-pixel basis, we're done.
	if (level < 1) return;

    // Diamond Step:    
    //   Average the four corners of a square and add a random offset to the centerpoint.
    for (unsigned int i = x1 + level; i < x2; i += level) {
        for (unsigned int j = y1 + level; j < y2; j += level) {
            float botLeft = getHeightmap( i - level, j - level );
            float botRight = getHeightmap( i, j - level );
            float topLeft = getHeightmap( i - level, j );
            float topRight = getHeightmap( i, j );
            setHeightmap( i - level / 2, j - level / 2, (topLeft + topRight + botLeft + botRight) / 4 + drand() * range );
        }
    }

    // Square Step:
    //    Set the heightmap values of the midpoints along each edge of each square in the heightmap (with side length = level)
    for (unsigned int i = x1 + 2 * level; i < x2; i += level) {
        for (unsigned int j = y1 + 2 * level; j < y2; j += level) {
            float a = getHeightmap(i - level, j - level);
            float b = getHeightmap(i, j - level);
            float c = getHeightmap(i - level, j);
            float d = getHeightmap(i, j);
            float e = getHeightmap(i - level / 2, j - level / 2);

            // The offsets are a little non-intuitive, but this way we won't reach out into memory we don't have
            setHeightmap( i - level, j - level / 2, (a + c + e + getHeightmap( i - 3 * level / 2, j - level / 2)) / 4 + drand() * range );
            setHeightmap( i - level / 2, j - level, (a + b + e + getHeightmap( i - level / 2, j - 3 * level / 2)) / 4 + drand() * range );
        }
    }

    // Random range has to fall off by 2 ^ (-roughness)
    diamondSquare(x1, y1, x2, y2, range / pow(2, roughness), level / 2, roughness);
}

void Terrain::render() {

	// Draw the terrain
	glBegin(GL_QUADS);
	Vector3 p0, p1, p2, p3, c0, c1, c2, c3, n0, n1, n2, n3;
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			
			// Grab vertices
			p0 = vertices.at( j * tesselX + i );
			p1 = vertices.at( j * tesselX + i + 1 );
			p2 = vertices.at( (j+1) * tesselX + i );
			p3 = vertices.at( (j+1) * tesselX + (i+1) );

			// Grab colors
			c0 = colors.at(j * tesselX + i);
			c1 = colors.at(j * tesselX + i + 1);
			c2 = colors.at((j + 1) * tesselX + i);
			c3 = colors.at((j + 1) * tesselX + (i + 1));

			// Grab normals
			n0 = normals.at( j * tesselX + i );
			n1 = normals.at( j * tesselX + i + 1 );
			n2 = normals.at( (j+1) * tesselX + i );
			n3 = normals.at( (j+1) * tesselX + (i+1) );

			glNormal3f(n0.getX(), n0.getY(), n0.getZ());
			glColor3f(c0.getX(), c0.getY(), c0.getZ());
			glVertex3f(p0.getX(), p0.getY(), p0.getZ());

			glNormal3f(n1.getX(), n1.getY(), n1.getZ());
			glColor3f(c1.getX(), c1.getY(), c1.getZ());
			glVertex3f(p1.getX(), p1.getY(), p1.getZ());

			glNormal3f(n3.getX(), n3.getY(), n3.getZ());
			glColor3f(c3.getX(), c3.getY(), c3.getZ());
			glVertex3f(p3.getX(), p3.getY(), p3.getZ());

			glNormal3f(n2.getX(), n2.getY(), n2.getZ());
			glColor3f(c2.getX(), c2.getY(), c2.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());

		}
	}
	glEnd();

}

// Return the vertices vector
std::vector<Vector3> * Terrain::getVertices() {
  return &vertices;
}