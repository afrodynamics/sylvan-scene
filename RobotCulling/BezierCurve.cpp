#include <exception>
#include <cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include <GL/glut.h>    // Windows and Linux are normal
#endif
#include "BezierCurve.h"

using namespace std;

BezierCurve::BezierCurve() {
	// No-arg constructor sets all controls points to 0,0,0
	p0 = p1 = p2 = p3 = Vector4(0,0,0,1.0);
};

BezierCurve::BezierCurve(Vector4 a, Vector4 b, Vector4 c, Vector4 d) {
	p0 = a;
	p1 = b;
	p2 = c;
	p3 = d;
};

BezierCurve::~BezierCurve() {

};

void BezierCurve::updateControlPoints(Vector4 *a, Vector4 *b, Vector4 *c, Vector4 *d) {
	// Only update our control points where params are not null
	if ( a != nullptr ) {
		p0 = *a;
	}
	if ( b != nullptr ) {
		p1 = *b;
	}
	if ( c != nullptr ) {
		p2 = *c;
	}
	if ( d != nullptr ) {
		p3 = *d;
	}
	// Exists just in case we want to do some precomputation here
	// later (i.e using matrix form.) IDC if it is slow for now
}

Vector4 BezierCurve::calcPoint(double t) {
	if ( t < 0 || t > 1 ) {
		//throw runtime_error("Cannot calculate point on spline with parameter outside range [0,1]");
		if (t < 0) return p0;
		if (t > 1) return p3;
	}
	// Copy our control points into temporaries so we don't modify them
	// with scale
	Vector4 q0(p0);
	Vector4 q1(p1);
	Vector4 q2(p2);
	Vector4 q3(p3);

	// Calculate the bezier coefficients using the cubic Bernstein
	// polynomials
	double tSquared, tCubed;
	double coeff0, coeff1, coeff2; // Third coeff is just t^3
	tSquared = pow(t, 2);
	tCubed = pow(t, 3);
	coeff0 = -tCubed + 3.0 * tSquared - 3.0 * t + 1;
	coeff1 = 3.0 * tCubed - 6.0 * tSquared + 3.0 * t;
	coeff2 = -3.0 * tCubed + 3.0 * tSquared;
	q0.scale(coeff0);
	q1.scale(coeff1);
	q2.scale(coeff2);
	q3.scale(tCubed);
	return ( q0 + q1 + q2 + q3 );

}

void BezierCurve::draw(double samples) {
	// Draws a Bezier curve using uniform sampling
	int t = 0;
	double t_inc = 1.0 / samples;
	Vector4 pt;
	glBegin(GL_LINE_STRIP);
	while ( t < samples ) {
		pt = calcPoint( t / samples );
		glVertex3d( pt.getX(), pt.getY(), pt.getZ() );
		t++; // Move forward
	}
	glEnd();
}