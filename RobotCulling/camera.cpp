/**
 * This is a class that camera matrix and the inverse
 * camera matrix. Please note that the inverse camera
 * matrix is calculated once and stored in column major
 * order. If you need to make repeated transformations
 * to the Camera matrix you may need to refactor this class.
 */
#include "Camera.h"

/**
 * Default Camera constructor just initialized the important 
 * matrices to the identity.
 */
Camera::Camera() {
	camera2world.identity();
	world2camera.identity();
	center = Vector3();
};

/**
 * Constructs a camera matrix from the mathematical defintion
 */
Camera::Camera( Vector3 centerOfProjection, Vector3 lookAtPoint, Vector3 upVector ) {

  Vector3 e = centerOfProjection;
  Vector3 d = lookAtPoint;
  Vector3 up = upVector;
  Vector3 x_c, y_c, z_c;
  Vector3 zero = Vector3(0,0,0);
  Vector3 one = Vector3(1,1,1);

  this->center = e; // So we can access it later

  z_c = e - d;
  z_c.normalize();
  x_c.cross( up, z_c ); // Crosses up and z_c, placing them in x_c
  x_c.normalize();
  y_c.cross( z_c, x_c );
  y_c.normalize(); // this vector should already be normal, but let's be safe

  this->up = y_c;
  this->right = x_c;
  this->lookDir = z_c;
  
  camera2world = Matrix4( x_c, y_c, z_c, e ); // TA left it at x_c, y_c, z_c, zero
  camera2world.transpose();
  
  // Calculate the inverse of the camera2world matrix
  
  Matrix4 rInverse = Matrix4( x_c, y_c, z_c, zero );   // TA says I don't need this 
  rInverse.transpose();                                // TA says I don't need this

  // The inverse of a translation matrix T is equal to negating
  // the 4th column of T. Since the fourth column is e, we'll
  // just construct T using the negated values of e
  Matrix4 tInverse = Matrix4::translate( -e.getX(), -e.getY(), -e.getZ() );

  //      C^-1    =     R^-1   *     T^-1
  // world2camera =  rInverse  X   tInverse
  
  world2camera = rInverse * tInverse; // should be in row-major order
  world2cameraRowMajor = world2camera;
  world2camera.transpose();           // now it's in column-major order

};

/**
 * Returns a reference to the column major inverse camera matrix
 * @return a reference to the column major inverse camera matrix
 */
Matrix4 Camera::getGlMatrix() {
  return world2camera;
};

/**
* Returns a reference to the row major inverse camera matrix
*/
Matrix4 Camera::getInverseMatrix() {
	return world2cameraRowMajor;
};

Vector3 Camera::getPos() {
	return center;
};

Vector3 Camera::getUp() {
	return up;
};

Vector3 Camera::getRight() {
	return right;
};

Vector3 Camera::getLookDir() {
	return lookDir;
};