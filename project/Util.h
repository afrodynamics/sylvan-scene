/**
 * A quick cross-compilable utility class
 * which defines a bunch of useful static functions
 * that don't really have a place in a single
 * class per se. Not supposed to be instantiated
 */
#pragma once
#include <cmath>
#include "Vector3.h"
#include "Vector4.h"

namespace Util {

	// Linearly interpolates between 2 doubles, vectors, whatever. 
	double lerp(double t, double p0, double p1);
	Vector3 lerp(double t, Vector3 p0, Vector3 p1);
	Vector4 lerp(double t, Vector4 p0, Vector4 p1);

	// Clamp a number between max & min
	double clamp(double val, double valMin, double valMax); 

	// Returns absolute value of a number
	double abs(double d);
};