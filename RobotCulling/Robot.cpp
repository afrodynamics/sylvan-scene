#include "Robot.h"


Robot::Robot()
{
	mtx = new Matrix4();
	mtx->identity();
	x = y = z = 0;
	createRobot();
}

Robot::Robot(Matrix4& copy)
{
	mtx = new Matrix4(copy);
	Vector4 pos = *mtx * Vector4(0, 0, 0, 1);
	x = pos.getX();
	y = pos.getY();
	z = pos.getZ();
	createRobot();
}

Robot::Robot(double x, double y, double z)
{
	mtx = new Matrix4(Matrix4::translate(x,y,z));
	this->x = x;
	this->y = y;
	this->z = z;
	createRobot();
}

Robot::~Robot()
{
	delete mtx;
	for (auto iter = children.begin(); iter != children.end(); ++iter) {
		delete (*iter); // Delete the child nodes
	}
}

/**
 * Creates the geometry required for the robot
 * and updates the appropriate pointers.
 */
void Robot::createRobot() {

	leftLegJoint = new MatrixTransform(Matrix4::translate(-1.1, -4, 0)  * Matrix4::scale(1, 2, 1));
	rightLegJoint = new MatrixTransform(Matrix4::translate(1.1, -4, 0) * Matrix4::scale(1, 2, 1));
	leftArmJoint = new MatrixTransform(Matrix4::translate(-3.5, 0, 0)  * Matrix4::scale(1, 2, 1));
	rightArmJoint = new MatrixTransform(Matrix4::translate(3.5, 0, 0) * Matrix4::scale(1, 2, 1));
	neckJoint = new MatrixTransform(Matrix4::translate(0, 5, 0));
	leftLeg = new Cube(2);
	rightLeg = new Cube(2);
	rightArm = new Cube(2);
	leftArm = new Cube(2);
	torso = new Cube(5);
	head = new Sphere(2.5, 8, 8);

	// Add the joints to the Robot (which is basically just a MatrixTransform)
	// We're just calling addChild on the top level of the object's local tree,
	// which in this case is just the 'this' pointer
	addChild(leftLegJoint);
	addChild(leftArmJoint);
	addChild(rightLegJoint);
	addChild(rightArmJoint);
	addChild(neckJoint);
	addChild(torso);

	leftLegJoint->addChild(leftLeg);
	leftArmJoint->addChild(leftArm);
	rightArmJoint->addChild(rightArm);
	rightLegJoint->addChild(rightLeg);
	neckJoint->addChild(head);

}

/**
 * Called by displayCallback to update the
 * position of this robot's limbs, etc.
 */
void Robot::animate() {
	mtx->transformLocal( Matrix4::rotY(.125) );
}