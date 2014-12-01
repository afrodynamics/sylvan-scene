#pragma once
#include <vector>
#include "Geode.h"
#include "BezierCurve.h"
#define MAX_CONTROL_POINTS 16

/**
 * Like all Geode derivatives, this patch is centered at its own origin
 * and has unit length, so we can make use of scaling MatrixTransforms
 * (it also simplifies having to pass in 16 control points...)
 *
 * By default, control points will be set in the X/Z plane
 */
class BezierPatch : public Geode {
private:
	// Holds the internal control points & Bezier Curves
	std::vector<Vector4> points;
	std::vector<Vector4> originalPoints; 
	// Unmodifed from ctor, used for the offset of sin/cos in animate

	// Given a patch in the XZ plane, let the X direction be the U axis
	// and the Z direction be the V axis.
	// >>> Each curve is defined as being further along the u/v axis, which 
	//     the curve itself is perpendicular to
	BezierCurve uCurve0, uCurve1, uCurve2, uCurve3;
	BezierCurve vCurve0, vCurve1, vCurve2, vCurve3;
	int samples;
	double radians;
public:
	BezierPatch();
	~BezierPatch();

	Vector4 calcPoint(double u, double v);
	// Changes control points of all the internal curves. Use if we modify
	// a control point (i.e animation)
	void updateCurves();
	void animate();
	void render(); // Inherited from Geode, draws the patch
	void setUnifSampling(int);
};