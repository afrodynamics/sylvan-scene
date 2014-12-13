#include <iostream>
#include <cstdlib>
#include "Util.h"
#include "Terrain.h"

using namespace std;
using namespace Util;

Terrain::Terrain() {

	// Seed the RNG
	srand(time(NULL)); // 198723

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
	//heightmap.resize( tesselX * tesselZ );
	for ( int i = 0; i < tesselX; ++i ) {
		for ( int j = 0; j < tesselZ; ++j ) {
			//setHeightmap(i,j,0.0);
            heightmap.push_back(0.0);
		}
	}
	
	diamondSquare(0,0,tesselX - 1,tesselZ - 1,.125);

};

// Getters and setters for the heightmap array
void Terrain::setHeightmap(int x, int y, double d) {
   heightmap[ y * tesselZ + x ] = d;
};

double Terrain::getHeightmap(int x,int y) {
	return heightmap.at(y * tesselZ + x);
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
 * I referred to some other sources to verify my understanding of the algorithm:
 * http://www.paulboxley.com/blog/2011/03/terrain-generation-mark-one
 * 
 * @param  top       y coordinate of the current square
 * @param  left      x coordinate of the current square
 * @param  bottom    bottom y coordinate of the current square
 * @param  right     right x coordinate of the current square
 * @param  randRange positive double which clamps our random displacement
 * @return           height value at the midpoint in the square
 */
void Terrain::diamondSquare(int top, int left, int bottom, int right, double randRange) {
	
	int centerX = (top + bottom) / 2;
	int centerY = (left + right) / 2;

	// Check out of bounds conditions
	if ( top < 0 || top > tesselZ || 
		 left < 0 || left > tesselX || 
		 right < 0 || right > tesselX || 
		 bottom < 0 || bottom > tesselZ || 
		 centerX < 0 || centerX > tesselX || 
		 centerY < 0 || centerY > tesselZ ) {
		return;
	}

	// During the diamond step, we set the center point to the
	// average of the four corner heights +/- some random delta
	
	double centerH = 
		( getHeightmap(left, top)
		+ getHeightmap(left, bottom)
		+ getHeightmap(right, bottom)
		+ getHeightmap(right, top)) / 4.0
	    + drandRange( -randRange, randRange );

	setHeightmap(centerX, centerY, centerH);

	// The square step uses the newly found center point to divide
	// the region into four quadrants. We fill in the heights for
	// the missing corners of the smaller squares, then recurse
	
	setHeightmap(centerX, top,     (getHeightmap(left,  top)    + getHeightmap(right, top)   ) / 2 + ((drand() - 0.5) * centerH));
	setHeightmap(centerX, bottom,  (getHeightmap(left,  bottom) + getHeightmap(right, bottom)) / 2 + ((drand() - 0.5) * centerH));
	setHeightmap(left,    centerY, (getHeightmap(left,  top)    + getHeightmap(left,  bottom)) / 2 + ((drand() - 0.5) * centerH));
	setHeightmap(right,   centerY, (getHeightmap(right, top)    + getHeightmap(right, bottom)) / 2 + ((drand() - 0.5) * centerH));

	// Loop condition
	if ((right - left) > 2 && (bottom - top) > 2) {
		diamondSquare( top, left, centerY, centerX, randRange );
		diamondSquare( top, centerX, centerY, right, randRange );
		diamondSquare( centerY, left, bottom, centerX, randRange );
		diamondSquare( centerY, centerX, bottom, right, randRange );
	}

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
			vertices.push_back( Vector3( 
				lerp(tx, minX, maxX), 
				getHeightmap(x, z), 
				lerp(tz, minZ, maxZ) ) 
			);
		}
	}
}

void Terrain::render() {

	// Draw the terrain
	glBegin(GL_QUADS);
	Vector3 p0, p1, p2, p3, p4, p5;
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			
			p0 = vertices.at( j * tesselX + i );
			p1 = vertices.at( j * tesselX + i + 1 );
			p2 = vertices.at( (j+1) * tesselX + i );
			p3 = vertices.at( (j+1) * tesselX + (i+1) );

			glVertex3f(p0.getX(), p0.getY(), p0.getZ());
			glVertex3f(p1.getX(), p1.getY(), p1.getZ());
			glVertex3f(p3.getX(), p3.getY(), p3.getZ());
			glVertex3f(p2.getX(), p2.getY(), p2.getZ());

		}
	}
	glEnd();

}