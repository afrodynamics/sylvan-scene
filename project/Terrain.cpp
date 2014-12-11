#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Terrain.h"

using namespace std;

Terrain::Terrain() {
	tesselX = tesselZ = 20;
	srand(198723);
	generate(); // Generate vertices for the terrain
};

Terrain::~Terrain() {
	// Nothing to destruct
}

void Terrain::generate() {
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
			double yRand = 0.0;
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

double Terrain::lerp(double t, double p0, double p1) {
	return ((1 - t) * p0 + t * p1);
};