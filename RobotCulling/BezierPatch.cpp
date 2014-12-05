#include <iostream>
#include <cmath>
#include <vector>
#include "BezierPatch.h"
#include "Window.h"

using namespace std;

BezierPatch::BezierPatch() {
	
	// Create a patch in the XZ plane
	samples = 100;
	radians = 0.0;
	boundingRadius = .5; // For "culling" even though it doesn't work

	// Define control points
	// for (double zz = minZ; zz < maxZ; zz += maxZ / 2.0 ) {
	// 	for (double xx = minX; xx < maxX; xx += maxX / 2.0 ) {
	// 		points.push_back(Vector4( xx, 0.0, zz, 1.0 ));
	// 	}
	// }
	// Don't use doubles in for loops
	for ( int zz = 0; zz < 4; ++zz) {
		for ( int xx = 0; xx < 4; ++xx) {
			double xNormalized = (double)xx / 3.0;
			double zNormalized = (double)zz / 3.0;
			xNormalized -= .5;
			zNormalized -= .5; // Center points around 0,0,0
			Vector4 vec4 = Vector4( xNormalized, 0.0, zNormalized, 1.0 );
			vec4.print("Patch Control Point: ");
			points.push_back(vec4);
		}
	}

	if ( points.size() != MAX_CONTROL_POINTS ) {
		// DEBUG
		std::cerr << "Created Bezier Patch with insufficient # of control points!" << std::endl;
	}

	// V Curves are parallel to the U axis (but they get further from each other in the V direction)
	vCurve0 = BezierCurve( points[0], points[1], points[2], points[3] );
	vCurve1 = BezierCurve( points[4], points[5], points[6], points[7] );
	vCurve2 = BezierCurve( points[8], points[9], points[10], points[11] );
	vCurve3 = BezierCurve( points[12], points[13], points[14], points[15] );

	// U Curves are "vertical" if we are looking straight down Z (parallel to V)
	uCurve0 = BezierCurve( points[0], points[4], points[8], points[12] );
	uCurve1 = BezierCurve( points[1], points[5], points[9], points[13] );
	uCurve2 = BezierCurve( points[2], points[6], points[10], points[14] );
	uCurve3 = BezierCurve( points[3], points[7], points[11], points[15] );

	originalPoints = std::vector<Vector4>(points);

}

// Destructor
BezierPatch::~BezierPatch() {

}

// Setter for uniform sampling variable
void BezierPatch::setUnifSampling(int s) {
	if ( s != 0 ) { // Dividing by zero is not cool, bro
		samples = s;
	}
}

// First calculate control points parallel to U, then construct a new curve
// to find the actual position of this vertex along the patch
Vector4 BezierPatch::calcPoint(double u, double v) {
	// Technically we only need 4 Bezier curves in this class, but oh well
	Vector4 q0, q1, q2, q3;
	q0 = vCurve0.calcPoint(u);
	q1 = vCurve1.calcPoint(u);
	q2 = vCurve2.calcPoint(u);
	q3 = vCurve3.calcPoint(u);

	BezierCurve b(q0,q1,q2,q3);
	return b.calcPoint(v);
}

// Calculate the normal at the point on the patch specified by u,v
Vector3 BezierPatch::calcNormal(double u, double v, Vector4 pt) {
	double delta = 0.0001;
	Vector4 tan_u = calcPoint(u + delta, v) - pt;
	Vector4 tan_v = calcPoint(u, v + delta) - pt;
	Vector3 tan_u3 = Vector3( tan_u.getX(), tan_u.getY(), tan_u.getZ() );
	Vector3 tan_v3 = Vector3( tan_v.getX(), tan_v.getY(), tan_v.getZ() );
	tan_u3.normalize();
	tan_v3.normalize(); // Avoid floating point issues
	tan_u3.cross(tan_u3, tan_v3);
	// tan_u3.normalize();
	return tan_u3;
}

// If we update/modify control points, we should update all the curves
// since I'm lazy and don't want to write lots of if/elses
void BezierPatch::updateCurves() {
	// V Curves are parallel to the U axis (but they get further from each other in the V direction)
	vCurve0.updateControlPoints( &points[0], &points[1], &points[2], &points[3] );
	vCurve1.updateControlPoints( &points[4], &points[5], &points[6], &points[7] );
	vCurve2.updateControlPoints( &points[8], &points[9], &points[10], &points[11] );
	vCurve3.updateControlPoints( &points[12], &points[13], &points[14], &points[15] );

	// U Curves are "vertical" if we are looking straight down Z (parallel to V)
	uCurve0.updateControlPoints( &points[0], &points[4], &points[8], &points[12] );
	uCurve1.updateControlPoints( &points[1], &points[5], &points[9], &points[13] );
	uCurve2.updateControlPoints( &points[2], &points[6], &points[10], &points[14] );
	uCurve3.updateControlPoints( &points[3], &points[7], &points[11], &points[15] );
}

void BezierPatch::animate() {

	double x, y, z;
	double period = 64; // Good #s: 16,
	radians += M_PI / 1024; // Not delta timing on my own, screw that
	double amplitude = 2;
	
	// First half of the control points will add the function
	for (unsigned int i = 0; i < MAX_CONTROL_POINTS / 2; ++i) {
		x = originalPoints[i].getX();
		y = originalPoints[i].getY();
		z = originalPoints[i].getZ();

		y += amplitude * (sin( period * radians ) - cos( period * radians ))/100;

		points[i] = Vector4(x,y,z,1.0);
	}
	// Second half of the control points will subtract the function
	for (unsigned int i = MAX_CONTROL_POINTS / 2; i < MAX_CONTROL_POINTS; ++i) {
	
		x = originalPoints[i].getX();
		y = originalPoints[i].getY();
		z = originalPoints[i].getZ();

		//y -= amplitude * ( sqrt(x*x + z*z) * sin( period * radians ) - cos( period * radians ))/100;
		y -= amplitude * (sin( period * radians ) - cos( period * radians ))/100;

		points[i] = Vector4(x,y,z,1.0);
	}
	if ( radians > 2 * M_PI ) {
		radians = 0.0;
	}
	// We want to leave the 4 corner/end control points unchanged
	points[0] = originalPoints[0];
	points[3] = originalPoints[3];
	points[12] = originalPoints[12];
	points[15] = originalPoints[15];

	updateCurves(); // If we make modifications to points, propagate them to the curves
}

void BezierPatch::render() {
	
	// Patch drawing algorithm (we want to draw quads in counter clockwise order)
	//  1. get the four corners of the square we want to "stitch"
	//  2. draw that quad, then iterate to the next quad
	//   (for now, we'll just have to suffer recomputing stuff, but we can optimize
	//   later if we need to optimize)

	double u = 0.0;
	double v = 0.0;
	double inc = 1.0 / (double)samples;
	double maxParam = 1.0 - inc;
	Vector4 q0, q1, q2, q3;
	Vector3 q0_n, q1_n, q2_n, q3_n;

	// U Axis is parallel to Z
	// V Axis is parallel to X

	glColor4f(0.0,0.35,.9,.8); // Make transparent
	if ( !drawBoundingSphere ) {
		glBegin(GL_QUADS); // Because I know this will work
		for (u = 0.0; u < maxParam; u += inc ) {
			for (v = 0.0; v < maxParam; v += inc ) {
				// Now we have our parameters for our first coord
				// for this quad. Then we need to calculate the others
				
				q0 = calcPoint(u,v);
				q0_n = calcNormal(u,v,q0);
				q1 = calcPoint(u + inc, v);
				q1_n = calcNormal(u + inc, v, q1);
				q2 = calcPoint(u, v + inc);
				q2_n = calcNormal(u, v + inc, q2);
				q3 = calcPoint(u + inc, v + inc);  
				q3_n = calcNormal(u + inc, v + inc, q3);

				// Flip the normals (test)
				q0_n.negate(); q1_n.negate(); q2_n.negate(); q3_n.negate();

				// because of maxParam, q3 in last iteration will have
				//  (u, v) = ( 1.0, 1.0 ) which is what we want
				
				// Counter clockwise order: for some reason
				//   this defines the triangles as "back facing"
				//   even though it should be front facing...
				//   so it's drawing the underside of the patch
				// 
				// My U/V axes were not parallel to what I thought they
				// were parallel to

				glNormal3f(q0.getX(), q0.getY(), q0.getZ());
				glVertex3f(q0.getX(), q0.getY(), q0.getZ());

				glNormal3f(q2.getX(), q2.getY(), q2.getZ());
				glVertex3f(q2.getX(), q2.getY(), q2.getZ());

				glNormal3f(q3.getX(), q3.getY(), q3.getZ());	
				glVertex3f(q3.getX(), q3.getY(), q3.getZ());

				glNormal3f(q1.getX(), q1.getY(), q1.getZ());
				glVertex3f(q1.getX(), q1.getY(), q1.getZ());
					
			}
		}
		glEnd();
	}
	else {
		// Draw "wireframe" of patch
		updateCurves();
		uCurve0.draw(samples); uCurve1.draw(samples); uCurve2.draw(samples); uCurve3.draw(samples);
		vCurve0.draw(samples); vCurve1.draw(samples); vCurve2.draw(samples); vCurve3.draw(samples);
	}

	animate();
}