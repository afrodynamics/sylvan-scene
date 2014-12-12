/**
 * Implementation of the functions found in Util.
 */
#include "Util.h"

// Linear interpolation
double Util::lerp(double t, double p0, double p1) {
	return ((1 - t) * p0 + t * p1);
};
Vector3 Util::lerp(double t, Vector3 p0, Vector3 p1) {
	return ( p0 * (1 - t) + p1 * t);
};
Vector4 Util::lerp(double t, Vector4 p0, Vector4 p1) {
	return ( p0 * (1 - t) + p1 * t);
};

// Keeps a number between the max/min values
double Util::clamp(double val, double valMin, double valMax) {
	if ( val < valMin ) {
		return valMin;
	}
	else if ( val > valMax ) {
		return valMax;
	}
	else {
		return val;
	}
};

// Absolute value
double Util::abs(double d) {
  if ( d >= 0 ) {
    return d;
  }
  else {
    return -d;
  }
}