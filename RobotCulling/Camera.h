#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
private:
	Vector3 center, up, right, lookDir;
public:
	// Vector3 e, d, up; // Where e is the center of projection, d is the point we are looking toward
	                  // and up is a vector pointing up (in world coordinates)
	Matrix4 camera2world; // The camera matrix, in row-major order
	Matrix4 world2camera; // this will hold the inverse of the camera matrix (row-major order)
	Matrix4 world2cameraRowMajor;

	// Default constructor constructs an identity matrix for camera2world & world2camera
	Camera();

	// The constructor constructs the camera matrix according to the math. definition
	Camera( Vector3 centerOfProjection, Vector3 lookAtPoint, Vector3 upVector ); 
	Matrix4 getGlMatrix();  // returns the camera matrix to OpenGL in col-major order
	Matrix4 getInverseMatrix(); // returns the inverse camera matrix in row-major order
	Vector3 getPos();
	Vector3 getUp();
	Vector3 getRight();
	Vector3 getLookDir();

    // TODO: Define camera transformation operations, which will wrap the Matrix4 methods
    //       so that we can change the glCamera2world matrix whenever we modify the camera matrix

};

#endif