#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Terrain.h"

using namespace std;

Terrain::Terrain() {

	// Seed the RNG
	srand(time(0)); // 198723

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
			setHeightmap(i,j,(float)rand()/(float)RAND_MAX);
		}
	}
	
	// Let's see what happens
	for ( int i = 0; i < tesselZ; ++i ) {
		midpointDisplacement(0, i, tesselX, i, .5, .8, subdivisions );
	}
	//diamondSquare(0, 0, tesselX, tesselZ, .5 );
};

// Getters and setters for the heightmap array
void Terrain::setHeightmap(int x, int y, double d) {
   heightmap[ y * tesselX + x ] = d;
};

double Terrain::getHeightmap(int x,int y) {
   return heightmap[ y * tesselX + x ];
};

// Midpoint Displacement algorithm
// takes line segment (x1, y1, x2, y2) and initial random range
void Terrain::midpointDisplacement(int x1, int y1, int x2, int y2, double randRange, double rough, int iterations ) {
	int xDist = x2 - x1;
	int yDist = y2 - y1;
	int xMid = xDist/2;
	int yMid = yDist/2;
	double avgHeight = lerp( 0.5, getHeightmap(x1, y1), getHeightmap(x2, y2) );
	double h = clamp( (float)rand()/(float)RAND_MAX, -randRange, randRange) + avgHeight;
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
 * A 2D application of the midpoint displacement algorithm
 * @param  top       y coordinate of the current square
 * @param  left      x coordinate of the current square
 * @param  bottom    bottom y coordinate of the current square
 * @param  right     right x coordinate of the current square
 * @param  randRange positive double which clamps our random displacement
 * @return           height value at the midpoint in the square
 */
double Terrain::diamondSquare(int top, int left, int bottom, int right, double randRange) {
	
	int sideWidth = (bottom - top) / 2;
	while (sideWidth > 0) {

		// Do the square step for each square
		for (int i = 0; i < tesselX; i+=sideWidth) {
			for (int j = 0; j < tesselZ; j+=sideWidth) {
				int rightOffset = i + sideWidth;
				int botOffset = j + sideWidth;
				int midX = (rightOffset - i) / 2;
				int midY = (botOffset - j) / 2;
				double displacement = clamp(rand() / (double)RAND_MAX, -randRange, randRange);
				setHeightmap(midX, midY, displacement);
			}
		}

		// Do the diamond step for each square
		for (int i = sideWidth / 2; i < tesselX; i+=sideWidth) {
			for (int j = sideWidth / 2; j < tesselZ; j+=sideWidth) {
				int rightOffset = i + sideWidth;
				int botOffset = j + sideWidth;
				int midX = (rightOffset - i) / 2;
				int midY = (botOffset - j) / 2;
				double displacement = clamp(rand() / (double)RAND_MAX, -randRange, randRange);
				setHeightmap(midX, midY, displacement);
			}
		}

		// Perform the diamond step
		randRange /= 2;
		sideWidth /= 2; // Divide by two
	}

	return 0.0;
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
			double yRand = getHeightmap(x, z); // row major
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
double Terrain::clamp(double val, double valMin, double valMax) {
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