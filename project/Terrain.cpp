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
	//generateColorsFromHeightmap(Vector3(0.0, 1.0, 1.0), Vector3(1.0, 0.0, 0.0)); // blue - red
	generateColorsFromHeightmap(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0)); // grayscale
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
	setHeightmap(0, 0, drandRange(0.0, 0.5));
	setHeightmap(0, tesselZ - 1, drandRange(-.125, 0.125));
	setHeightmap(tesselX - 1, 0, drandRange(-.125, 0.125));
	setHeightmap(tesselX - 1, tesselZ - 1, drandRange(0.0, 0.5));
	
	double roughness = drandRange(.5,.55);
	double random = drand(); // default .0625
	diamondSquare(0,0,tesselX - 1,tesselZ - 1, random, roughness);

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
			//double t = Util::abs(getHeightmap(x, z));
			if (getHeightmap(x, z) > 0) {
				colors.push_back(lerp(t, valley, peak));
			}
			else {
				colors.push_back(valley); // If it's too low or negative, just use valley color
			}
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
 * I referred to some other sources to verify my understanding of the algorithm:
 * http://www.paulboxley.com/blog/2011/03/terrain-generation-mark-one
 * 
 * @param  top       y coordinate of the current square
 * @param  left      x coordinate of the current square
 * @param  bottom    bottom y coordinate of the current square
 * @param  right     right x coordinate of the current square
 * @param  randRange positive double which clamps our random displacement
 * @param  roughness constant between 0.0 and 1.0 that erodes our random range (roughest is 1, smoother toward 0)
 * @return           height value at the midpoint in the square
 */
void Terrain::diamondSquare(int top, int left, int bottom, int right, double randRange, double roughness) {
	
	// Stack overflow was happening because these values were swapped
	int centerX = (left + right) / 2;
	int centerY = (top + bottom) / 2;

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
	    //+ drandRange( -randRange, randRange );
		- drandRange(-randRange, randRange);

	// Experiment: clamp height between 0 and 1
	centerH = clamp( centerH, 0.0, 1.0 );

	if ( centerH > maxH ) {
		maxH = centerH;
	}
	else if ( centerH < minH ) {
		minH = centerH;
	}

	setHeightmap(centerX, centerY, centerH);

	// Apply roughness
	randRange *= roughness;

	// The square step uses the newly found center point to divide
	// the region into four quadrants. We fill in the heights for
	// the missing corners of the smaller squares, then recurse
	
	setHeightmap(centerX, top, clamp(
		(getHeightmap(left, top) + getHeightmap(right, top)) / 2 + drandRange(-randRange, randRange), /* * centerH */
		0.0, 1.0 )
	);
	setHeightmap(centerX, bottom, clamp(
		(getHeightmap(left, bottom) + getHeightmap(right, bottom)) / 2 + drandRange(-randRange, randRange), /* * centerH */
		0.0, 1.0 )
	);
	setHeightmap(left, centerY, clamp(
		(getHeightmap(left, top) + getHeightmap(left, bottom)) / 2 + drandRange(-randRange, randRange), /* * centerH */
		0.0, 1.0 )
	);
	setHeightmap(right, centerY, clamp(
		(getHeightmap(right, top) + getHeightmap(right, bottom)) / 2 + drandRange(-randRange, randRange), /* * centerH */
		0.0, 1.0 )
	);

	// If the next squares will have side lengths smaller than 2, we've covered all of the vertices
	if ((right - left) > 2) {
		// Lower the randRange
		diamondSquare(top, left, centerY, centerX, randRange, roughness );
		diamondSquare(top, centerX, centerY, right, randRange, roughness);
		diamondSquare(centerY, left, bottom, centerX, randRange, roughness);
		diamondSquare(centerY, centerX, bottom, right, randRange, roughness);
	}

}

void Terrain::render() {

	// Draw the terrain
	glBegin(GL_QUADS);
	Vector3 p0, p1, p2, p3, c0, c1, c2, c3, n0, n1, n2, n3;
	for ( int i = 0; i < tesselX - 1; ++i ) {
		for ( int j = 0; j < tesselZ - 1; ++j ) {
			
			p0 = vertices.at( j * tesselX + i );
			p1 = vertices.at( j * tesselX + i + 1 );
			p2 = vertices.at( (j+1) * tesselX + i );
			p3 = vertices.at( (j+1) * tesselX + (i+1) );
			c0 = colors.at(j * tesselX + i);
			c1 = colors.at(j * tesselX + i + 1);
			c2 = colors.at((j + 1) * tesselX + i);
			c3 = colors.at((j + 1) * tesselX + (i + 1));
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