#pragma once
#include "Matrix4.h"

class BezierCurve {
	// Defines a cubic Bezier Curve
private:
	Vector4 p0, p1, p2, p3;
public:
	BezierCurve(); // All control points at origin by default
	BezierCurve(Vector4, Vector4, Vector4, Vector4);
	~BezierCurve();
	void updateControlPoints(Vector4*,Vector4*,Vector4*,Vector4*);
	void draw(int); // Draws Bez. curve as linestrip with given # of samples

	// Returns true if this BezierCurve is C1 continuous with the beginning
	// of another BezierCurve (checks continuity at the end of this curve)
	bool c1Continuous(BezierCurve& next); 

	// Ensures that this BezierCurve is C1 continuous with the given BezierCurve
	// "next". The algorithm will try to preserve both curves, but expect both
	// to be modified!
	void makeContinuous(BezierCurve& next);

	// Calculates the point along the curve at t between [0.0, 1.0]
	Vector4 calcPoint(double t);
};