#include "BezierSpline.h"
#include <exception>

BezierSpline::~BezierSpline() {}

// Draw all the curves using uniform sampling
void BezierSpline::draw(int samples) {
	for (auto iter = curves.begin(); iter != curves.end(); ++iter) {
		(*iter).draw(samples);
	}
}

// Push a new BezierCurve onto the end of the loop. Once a curve
// has been "closed" we can no longer add curves to the spline.
// This is because of the pop() behavior, which attempts to force
// the next-to-last curve to loop with the first curve if we delete 
// the last curve. Adding curves to the end of a closed loop is weird.
void BezierSpline::push(BezierCurve& newCurve) {
    if(curves.size() == 0) {
        curves.push_back(newCurve);
    }
    else {
        if (closed) return;
        BezierCurve& lastCurve = curves.at( curves.size() - 1 );
        if ( lastCurve.c1Continuous( newCurve ) ) {
            curves.push_back( newCurve );
        }
        else {
            lastCurve.makeContinuous( newCurve );
            curves.push_back( newCurve );
        }
    }
}

// Pop a BezierCurve off the end of the spline. If the spline is in
// looping mode and has been closed, try to coerce the remaining
// curves into looping with the first curve of the spline
void BezierSpline::pop() {
	
	curves.pop_back(); // Delete the old spline 
	if ( closed && loop ) {
		if (curves.size() > 1 ) {
			// Make the last curve in the spline pick up the slack
			BezierCurve& lastCurve = curves.at( curves.size() - 1 );
			lastCurve.makeContinuous( curves.at(0) );
		}
	}
}

// Attempt to close the loop
void BezierSpline::closeLoop() {
	if (curves.size() > 1 ) {
		// Make the last curve in the spline pick up the slack
		BezierCurve& lastCurve = curves.at( curves.size() - 1 );
		lastCurve.makeContinuous( curves.at(0) );
	    loop = closed = true;
	}
	else {
		// We can't close a spline with only one curve, since a
		// cubic Bezier curve cannot be c1 continuous with itself
		// withough becoming a line.
		loop = closed = false;
	}
}

//calcuate point
Vector4 BezierSpline::calcPoint(double t) {
    if ( t < 0 || t > 1 ) {
        throw std::runtime_error("Cannot calculate point on spline with parameter outside range [0,1]");
    }
    double scaledT = t * curves.size();
    BezierCurve curveToDraw;
    
    int i = 0;
    for(i = 0; i < curves.size(); i++) {
        if(scaledT >= i && scaledT <= (i+1)) {
            curveToDraw = curves.at(i);
            break;
        }
    }
    return curveToDraw.calcPoint(scaledT - i);
}
