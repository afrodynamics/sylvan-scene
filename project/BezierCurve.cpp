#include <exception>
#ifdef __APPLE__
#include <GLUT/glut.h>  // Include GLUT header for Mac OS X
#else
#include "glee.h"
#include <GL/glut.h>    // Windows and Linux are normal
#endif
#include "BezierCurve.h"
#include "Util.h"

using namespace std;
using namespace Util;

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

// Returns true if this BezierCurve is C1 continuous with the beginning
// of another BezierCurve (checks continuity at the end of this curve)
bool BezierCurve::c1Continuous(BezierCurve& next) {

	Vector4 myEnd = p3;
	Vector4 otherEnd = next.p0;
	Vector3 mySlope = (p3 - p2).getVector3();
	Vector3 otherSlope = (next.p0 - next.p1).getVector3();

	// Normalize them so we're less likely to run into floating point issues
	mySlope.normalize();
	otherSlope.normalize();

	// The points p2, p3, other.p0, other.p1 must lie in the same line
	// Thus, they can all be represented as parallel vectors. The dot
	// product of two parallel vectors is 1.
	
	if ( myEnd.getX() == otherEnd.getX() && 
		 myEnd.getY() == otherEnd.getY() && 
		 myEnd.getZ() == otherEnd.getZ() && 
		 myEnd.getW() == otherEnd.getW() ) {
		return ( mySlope.dot(mySlope, otherSlope) == 1.0 );
	}
	else {
		// They aren't even C0 continuous :(
		return false;
	}

};

// Ensures that this BezierCurve is C1 continuous with the given BezierCurve
// "next". The algorithm will try to preserve both curves, but expect both
// to be modified!
void BezierCurve::makeContinuous(BezierCurve& next) {

	if (c1Continuous(next)) return; // We're already continuous, change nothing
	
	Vector3 mySlope = (p3 - p2).getVector3();
	Vector3 otherSlope = (next.p0 - next.p1).getVector3();
	double myDist = mySlope.length();
	double otherDist = mySlope.length();
	double total = myDist + otherDist;

	// We can do better than a simple average. If we lerp between our p2 and their
	// p1, using the ratio given by the distance to our endpoint over the total of the
	// distances between each curve and the desired endpoint (our p3) we can minimize
	// the changes we make to both curves.
	p3 = lerp( myDist / total, p2, next.p1 );
	next.p0 = p3;

};

Vector4 BezierCurve::calcPoint(double t) {
	if ( t < 0 || t > 1 ) {
		throw runtime_error("Cannot calculate point on spline with parameter outside range [0,1]");
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
	coeff0 = -tCubed + 3 * tSquared - 3 * t + 1;
	coeff1 = 3 * tCubed - 6 * tSquared + 3 * t;
	coeff2 = -3 * tCubed + 3 * tSquared;
	q0.scale(coeff0);
	q1.scale(coeff1);
	q2.scale(coeff2);
	q3.scale(tCubed);
	return ( q0 + q1 + q2 + q3 );

}

void BezierCurve::draw(int samples) {
	// Draws a Bezier curve using uniform sampling
	unsigned int i = 0;
	double t = 0.0;
	double t_inc = 1.0 / samples;
	Vector4 pt;
	glBegin(GL_LINE_STRIP);
	while ( i < samples ) {
		pt = calcPoint(t);
		glColor3f(0, 0, 0);
		glVertex3d( pt.getX(), pt.getY(), pt.getZ() );
		t += t_inc; // Move forward
		++i;
	}
	glEnd();
}
