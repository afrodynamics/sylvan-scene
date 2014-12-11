#include <iostream>
#include "Terrain.h"

using namespace std;

Terrain::Terrain() {
	tesselX = tesselZ = 100;
	generate(); // Generate vertices for the terrain
	cerr << "in Terrain, finished generate" << endl;
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

	for ( int x = 0; x < tesselX; ++x) {
		for ( int z = 0; z < tesselZ; ++z) {
			double tx = (double)x / (double)tesselX;
			double tz = (double)z / (double)tesselZ;
			double txNext = (double)(x+1) / (double)tesselX;
			double tzNext = (double)(z+1) / (double)tesselZ;
 			vertices.push_back( Vector3( lerp(tx, minX, maxX), 0.0, lerp(tzNext, minZ, maxZ) ) );
 			vertices.push_back( Vector3( lerp(txNext, minX, maxX), 0.0, lerp(tzNext, minZ, maxZ) ) );
 			vertices.push_back( Vector3( lerp(txNext, minX, maxX), 0.0, lerp(tz, minZ, maxZ) ) );
 			vertices.push_back( Vector3( lerp(tx, minX, maxX), 0.0, lerp(tz, minZ, maxZ) ) );
		}
	}
}

void Terrain::render() {

	// Draw the terrain
	glBegin(GL_QUADS);
	for ( auto iter = vertices.begin(); iter != vertices.end(); ++iter) {
		Vector3 pt = *iter;
		glVertex3f(pt.getX(), pt.getY(), pt.getZ());
	}
	glEnd();
}

double Terrain::lerp(double t, double p0, double p1) {
	return ((1 - t) * p0 + t * p1);
};