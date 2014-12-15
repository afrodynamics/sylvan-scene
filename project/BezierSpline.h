/**
 * A small wrapper for a piecewise Bezier spline. This class is designed
 * to be constructed once: push all the BezierCurves you want onto it, and
 * then close the loop (if you want the spline to loop) when you are finished.
 * If you pop off curves after closing the loop, the spline will attempt to
 * close the loop by moving curves around until there are one or fewer curves.
 */
#include <vector>
#include "BezierCurve.h"

class BezierSpline {
private:
	std::vector<BezierCurve> curves;
	bool loop;   // Indicates whether this spline should form a continuous loop
	bool closed; // Indicates whether the spline is a closed loop (used to prevent
		         // pushing any further curves.)
public:
	BezierSpline() : loop(false), closed(false) {};   // Splines don't loop by default
    ~BezierSpline();
	void draw(int); // Draw the curve in its entirety, given samples
	void push(BezierCurve&);
	void pop();
	void closeLoop(); // Prevent adding
    Vector4 calcPoint(double t);
};