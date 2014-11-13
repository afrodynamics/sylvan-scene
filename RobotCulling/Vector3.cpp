/***************************************************
 +-------------------------------------------------+
 |                     Vector3                     |
 +-------------------------------------------------+
 ***************************************************/

#include <iostream>
#include "Vector3.h"
#include "math.h"

/**
 * Constructor
 */
Vector3::Vector3(double x, double y, double z) {
	this->x = x; this->y = y; this->z = z;
};

/**
 * Default Constructor
 */
Vector3::Vector3() {
	this->x = this->y = this->z = 0;
};

/**
 * Plus Operator
 */
Vector3 Vector3::operator+(const Vector3& other) {
	return Vector3(
		this->x + other.x,
		this->y + other.y,
		this->z + other.z
	);
};

/**
 * Minus Operator
 */
Vector3 Vector3::operator-(const Vector3& other) {
	return Vector3(
		this->x - other.x,
		this->y - other.y,
		this->z - other.z
	);
};

/**
* Scalar Mult. Operator
*/
Vector3 Vector3::operator*(double other) {
	return Vector3(
		this->x * other,
		this->y * other,
		this->z * other
		);
};

/**
 * Negates the vector. (Multiplies the vector by -1)
 */
void Vector3::negate() {
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
};

/**
 * Scales the vector. (Multiplies the vector by a scalar)
 */
void Vector3::scale(double s) {
	this->x *= s; 
	this->y *= s; 
	this->z *= s;
}; 

/**
 * Takes two vectors and returns their dot products.
 */
double Vector3::dot(const Vector3& a, const Vector3& b) {
	return (a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z);
};

/**
 * Calculates the cross product and returns the result, as well
 * as putting it in the calling vector.
 */
Vector3 Vector3::cross(const Vector3& a, const Vector3& b) {
	this->x = (a.y)*(b.z) - (a.z)*(b.y);
	this->y = (a.z)*(b.x) - (a.x)*(b.z);
	this->z = (a.x)*(b.y) - (a.y)*(b.x);
	return *this;
};

double Vector3::length() {
	double magnitude = sqrt(pow(this->x, 2.0) + pow(this->y, 2.0) + pow(this->z, 2.0));
	return magnitude;
};

/**
 * Normalizes the calling vector
 */
void Vector3::normalize() {
	double l = this->length();
	if ( l == 1 || l == 0 ) return;
	this->x /= l;
	this->y /= l;
	this->z /= l;
};

void Vector3::print(std::string comment) {
	std::cerr << comment << " [ "
		<< this->x << ", "
		<< this->y << ", "
		<< this->z << " ]\n";
};

void Vector3::printAsMatlab() {
	std::cerr << " [ "
		<< this->x << " "
		<< this->y << " "
		<< this->z << " ]\n";
};

double Vector3::getX() { return this->x; };
double Vector3::getY() { return this->y; };
double Vector3::getZ() { return this->z; };
