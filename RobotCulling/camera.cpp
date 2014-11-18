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
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);
	lookDir = Vector3(0, 0, -1);
	center = Vector3(0,0,0);
	updateInverseCameraMtx();
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
  y_c = up;
  y_c.normalize();

  this->up = y_c;
  this->right = x_c;
  this->lookDir = z_c;
  
  camera2world = Matrix4( x_c, y_c, z_c, e ); // TA left it at x_c, y_c, z_c, zero
  
  // Calculate the inverse of the camera2world matrix
  updateInverseCameraMtx();

};

void Camera::updateInverseCameraMtx() {
	
	// This function assumes we've changed the camera matrix, camera2world
	// and creates a new inverse camera matrix based on R & T components

	Vector3 x_c = this->right;
	Vector3 y_c = this->up;
	Vector3 z_c = this->lookDir;
	Vector3 zero = Vector3(0, 0, 0);

	// Rotational component
	Matrix4 rInverse = Matrix4(x_c, y_c, z_c, zero);
	rInverse.transpose();

	// The inverse of a translation matrix T is equal to negating
	// the 4th column of T. Since the fourth column is e, we'll
	// just construct T using the negated values of e
	Matrix4 tInverse = Matrix4::translate(-center.getX(), -center.getY(), -center.getZ());

	//      C^-1    =     R^-1   *     T^-1
	// world2camera =  rInverse  X   tInverse

	world2camera = rInverse * tInverse; // should be in row-major order
	world2cameraRowMajor = world2camera;
	world2camera.transpose();           // now it's in column-major order

}

/*--------------------------------------------*
*                   Getters                  *
*--------------------------------------------*/

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