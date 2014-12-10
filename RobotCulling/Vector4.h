#ifndef _VECTOR4_H_
#define _VECTOR4_H_  // Don't doubly include the Vector4 class

#include <string>
#include "Vector3.h"

class Vector4 {
private:
	double x, y, z, w;
public:
	Vector4(); // Default constructor (initializes all values to 0)
	Vector4(double x, double y, double z, double w); // Constructor
	Vector4 operator+(const Vector4&); // Overload + operator for addition
	Vector4 operator-(const Vector4&); // Overlaod - operator for subtraction
	void dehomogenize();        // Scale the vector so that the fourth component (w) is equal to 1
	void print(std::string comment); // Print x, y, z, w components of the vector after a comment string
	void printAsMatlab();       // Prints vector out as a column vector in MATLAB. [x; y; z; w]
	                            // not required by prof, but probably good for testing?
    double length();            // returns magnitude of the vector
    double getX();
    double getY();
    double getZ();
    double getW();
    void negate();
    void normalize();
    void scale(double s);
    Vector3 getVector3();

    // Might be useful in the future
    bool isPoint();  // Returns true if this Vector4 represents a point (w = 1)
                     // can also be used to check if a Vector needs to be dehomogenized
    bool isVector(); // Returns true is w = 0, meaning this Vector4 represents a
                     // direction.

    // I tried writing getters, didn't work for 
    //   Matrix4::operator*(const Vector4&)
    friend class Matrix4;

}; 

#endif

