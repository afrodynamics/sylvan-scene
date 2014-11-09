#ifndef _VECTOR3_H_
#define _VECTOR3_H_  // Don't doubly include the Vector3 class

#include <string>

class Vector3 {
	friend class Matrix4;
protected:	
	double x, y, z;
public:
	Vector3(); // Default constructor (all zero)
	Vector3(double x, double y, double z); // Constructor
	Vector3 operator+ (const Vector3&); // Overload operator + for addition
	Vector3 operator- (const Vector3&); // Overload operator - for subtraction
	void negate();         // Implement vector negation (times -1)
	void scale(double s);  // Multiple the vector by scalar s
	//double dot(const Vector3&); // Returns the dot product of the calling vector, with parameter
	double dot(const Vector3&, const Vector3&); // Returns the result of the dot product
	Vector3 cross(const Vector3&, const Vector3&); // Returns the result and puts it in calling vector
	double length();  // computer the magnitude of the vector
	double getX();
	double getY();
	double getZ();
	void normalize(); // Normalize the vector (scale vector so its length is 1)
	void print(std::string comment); // Print x,y,z components of the vector, after a comment string
	void printAsMatlab();       // Print this vector as a MATLAB column vector [x; y; z]
};

#endif

