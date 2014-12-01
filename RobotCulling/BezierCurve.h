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
	void draw(double); // Draws Bez. curve as linestrip with given # of samples
	Vector4 calcPoint(double t);
};