/**
 * Implementation of the functions found in Util.
 */
#include "Util.h"

/** Math **/

// Linear interpolation
double Util::lerp(double t, double p0, double p1) {
	return ((1 - t) * p0 + t * p1);
};
// Component-wise linear interpolation
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

/** Data Structures **/

// Utilities for getting values from a row-major std::vector
// template <typename T>
// T Util::getItemFromRowMajorVector(std::vector<T>& array, int x, int y, int rows, int cols) {
// 	return array.at(y * rows + x);
// };

// template <typename T>
// void Util::setItemFromRowMajorVector(std::vector<T>& array, int x, int y, int rows, int cols, T item) {
// 	if ( y * rows + x < array.size()) {
// 		array.size[ y * rows + x ] = item;
// 	}
// };

/** Random **/

// Set the RNG seed. If parameter is zero, seed with time(NULL)
void Util::seed(int s) { ( s ? srand(s) : srand(time(NULL)) ); }

// Get a random double between 0.0 and 1.0
double Util::drand() { return (double)rand()/(double)RAND_MAX; }

// Get a random double between low and high, inclusive
double Util::drandRange( double low, double high ) { 
	return Util::lerp( Util::drand(), low, high ); // Lerp is love, lerp is life
};