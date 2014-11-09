/***************************************************
 +-------------------------------------------------+
 |                     Vector4                     |
 +-------------------------------------------------+
 ***************************************************/

#include <iostream>
#include <exception>
#include <math.h>
#include "Vector4.h"

using namespace std;

/**
 * Default Constructor
 */
Vector4::Vector4() {
	x = y = z = w = 0; // The beauty of one liners
}

/**
 * Standard Constructor
 */
Vector4::Vector4(double x, double y, double z, double w) {
	this->x = x; this->y = y; this->z = z; this->w = w;
	/*if (!( w == 0 || w == 1 )) {
    	throw new std::runtime_error("Please initialize homogenous component to 0 or 1!");
	}*/
}

/**
 * Plus Operator: 
 *   Returns the result of a Vector4 addition by value
 */
Vector4 Vector4::operator+(const Vector4& other) {
	return Vector4(
		this->x + other.x,
		this->y + other.y,
		this->z + other.z,
		this->w + other.w  // It's okay to do this so long as we call
		                   // dehomogenize afterwards if we need to!
	);
};

/**
 * Minus Operator: 
 *   Returns the result of a Vector4 subtraction by value
 */
Vector4 Vector4::operator-(const Vector4& other) {
	return Vector4(
		this->x - other.x,
		this->y - other.y,
		this->z - other.z,
		this->w - other.w  // It's okay to do this so long as we call
		                   // dehomogenize afterwards if we need to!
	);
};

/**
 * Dehomogenize:
 *   Scales this Vector4 so that the fourth component is equal to one.
 * This is achieved simply by dividing each component by w, where w is nonzero.
 */
void Vector4::dehomogenize() {
	if (this->w) {
		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w /= this->w;
	}
	else {
		cerr << "Warning, cannot dehomogenize Vector4 with w = 0\n";
	}
}

/**
 * Negate
 */
void Vector4::negate() {
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
	this->w = -this->w;
}

/**
 * Length
 */
double Vector4::length() {
	return sqrt( 
	  x * x +
	  y * y + 
	  z * z +
	  w * w
	);
}

/**
 * Normalize
 */
void Vector4::normalize() {
	this->x /= this->length();
	this->y /= this->length();
	this->z /= this->length();
	this->w /= this->length();
};

/**
 * isPoint:
 *   Returns true if this Vector4's w component is equal to one.
 *   This will return false if the Vector4 is a direction (w = 0)
 *   or if the Vector4 needs to be dehomogenized.
 */
bool Vector4::isPoint() { return ( w == 1 ); };

/**
 * isVector:
 *    Returns true if w = 0, meaning this Vector4 represents a 
 *    direction in homogenous coordinates. If it returns false,
 *    we can't be sure that the Vector4 is homogenized.
 */
bool Vector4::isVector() { return ( w == 0 ); };

/**
 * print:
 *  Prints out the values of this vector, prefixed with a comment str.
 */
void Vector4::print(string comment) {
	cerr << comment << " [ "
		<< this->x << ", "
		<< this->y << ", "
		<< this->z << ", "
		<< this->w << " ]\n";
}

/**
* printAsMatlab:
*  Prints out the values of this vector, as a MATLAB-compatible column vector.
*/
void Vector4::printAsMatlab() {
	cerr << " [ "
		<< this->x << "; "
		<< this->y << "; "
		<< this->z << "; "
		<< this->w << " ]\n";
}

/**
 * Standard getters
 */
double Vector4::getX() { return x; };
double Vector4::getY() { return y; };
double Vector4::getZ() { return z; };
double Vector4::getW() { return w; };

