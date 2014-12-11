#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Terrain.h"

using namespace std;

Terrain::Terrain() {

	// Seed the RNG
	srand(198723);

	// Prepare the resolution of the heightmap
	subdivisions = 6; // Be careful, exponential time going on here!
	tesselX = tesselZ = pow(2, subdivisions) + 1; // This will be important later

	// Do all the computation
	generateHeightmap();
	generateVerts(); // Generate vertices for the terrain
};

Terrain::~Terrain() {
	// Nothing to destruct
}

/**
 * Generates a heightmap by populating a square 2D array.
 * Heightmap is in ROW-MAJOR, so the pixel (X,Y) is
 *     heightmap[ Y * tesselX + X ]
 */
void Terrain::generateHeightmap() {
	heightmap.resize( tesselX * tesselZ );
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			heightmap[ j * tesselX + i ] = 0.0;
		}
	}
	heightmap[ tesselZ / 2 * tesselX + tesselZ / 2 ] = diamondSquare(0,0,tesselX,tesselZ,.5);
};

/**
 * An implementation of the diamond-square terrain generation algorithm.
 * Described in great detail here: http://www.gameprogrammer.com/fractal.html
 * A 2D application of the midpoint displacement algorithm
 * @param  top       y coordinate of the current square
 * @param  left      x coordinate of the current square
 * @param  bottom    bottom y coordinate of the current square
 * @param  right     right x coordinate of the current square
 * @param  randRange positive double which clamps our random displacement
 * @return           height value at the midpoint in the square
 */
double Terrain::diamondSquare(int top, int left, int bottom, int right, double randRange) {
	int midX = (bottom - top) / 2;
	int midY = (right - left) / 2;
	int quartX = midX / 2;
	int quartY = midY / 2;
	double displacement = clamp(rand() % 256, -randRange, randRange);

	// Recurse so long as we have space
	if ( randRange > .125 ) {
		// Top left square
		heightmap[ quartX * tesselX + quartY ] = diamondSquare(top, left, midY, midX, randRange / 2.0);
		// Top right square
		heightmap[ (midX + quartX) * tesselX + quartY ] = diamondSquare(top, midX, midY, right, randRange / 2.0);
		// Bottom left square
		heightmap[ (quartX) * tesselX + quartY + midY ] = diamondSquare(midY, left, bottom, midX, randRange / 2.0);
		// Bottom right square
		heightmap[ (midX + quartX) * tesselX + quartY + midY ] = diamondSquare(midY, midX, bottom, right, randRange / 2.0);
	}

	return heightmap[ midY * tesselX + midX ] + displacement;
}

void Terrain::generateVerts() {
	// Definitions
	double minX, minZ;
	double maxX, maxZ;
	minX = minZ = -.5;
	maxX = maxZ = .5;

	// Fill the vertices array
	for ( int x = 0; x < tesselX; ++x ) {
		for ( int z = 0; z < tesselZ; ++z ) {
			double tx = (double)x / (double)tesselX;
	 		double tz = (double)z / (double)tesselZ;
			double yRand = heightmap[ x * tesselZ + z ];
			vertices.push_back( Vector3( lerp(tx, minX, maxX), yRand, lerp(tz, minZ, maxZ) ) );
		}
	}

}

void Terrain::render() {

	// Draw the terrain
	glBegin(GL_QUADS);
	Vector3 p0, p1, p2, p3, p4, p5;
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			
			p0 = vertices[ j * tesselX + i ];
			p1 = vertices[ j * tesselX + i + 1 ];
			p2 = vertices[ (j+1) * tesselX + i ];
			p3 = vertices[ (j+1) * tesselX + (i+1) ];

			glVertex3f(p0.getX(), p0.getY(), p0.getZ());
			glVertex3f(p1.getX(), p1.getY(), p1.getZ());
			glVertex3f(p3.getX(), p3.getY(), p3.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());

		}
	}
	glEnd();

}

// Linear interpolation
double Terrain::lerp(double t, double p0, double p1) {
	return ((1 - t) * p0 + t * p1);
};

// Keeps a number between the max/min values
double Terrain::clamp(double val, double valMax, double valMin) {
	if ( val < valMin ) {
		return valMin;
	}
	else if ( val > valMax ) {
		return valMax;
	}
	else {
		return val;
	}
};