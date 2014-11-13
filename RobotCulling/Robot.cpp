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

	leftArmMtx = Matrix4::translate(-3.5, -2, 0)  * Matrix4::scale(1, 3, 1);
	rightArmMtx = Matrix4::translate(3.5, -2, 0) * Matrix4::scale(1, 3, 1);
	leftLegMtx = Matrix4::translate(-1.1, -5, 0)  * Matrix4::scale(1, 3, 1);
	rightLegMtx = Matrix4::translate(1.1, -5, 0) * Matrix4::scale(1, 3, 1);

	leftLegJoint = new MatrixTransform( leftLegMtx );
	rightLegJoint = new MatrixTransform( rightLegMtx );
	leftArmJoint = new MatrixTransform( leftArmMtx );
	rightArmJoint = new MatrixTransform( rightArmMtx );
	neckJoint = new MatrixTransform(Matrix4::translate(0, 5, 0));
	leftLeg = new Cube(2);
	rightLeg = new Cube(2);
	rightArm = new Cube(2);
	leftArm = new Cube(2);
	torso = new Cube(5);
	head = new Sphere(2.5, 8, 8);

	boundingRadius = 2;

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

	// Initialize the animation state
	rotateDir = rotateArmDir = -1;
	rotateSpeed = .125;
	leftArmAngle = 90;
	rightArmAngle = 270;
	leftLegAngle = rightLegAngle = 0;


	//mtx->transformLocal(Matrix4::rotY(45)); // something here is wrong!
}

/**
 * Called by displayCallback to update the
 * position of this robot's limbs, etc.
 */
void Robot::animate() {

	// spin the entire robot around
	mtx->transformLocal(Matrix4::rotY(rotateSpeed/10));
	
	// We're walking forward
	//mtx->transformWorld(Matrix4::translate(rotateSpeed / 100, 0, 0));

	leftArmAngle += rotateSpeed * rotateArmDir;
	rightArmAngle += rotateSpeed * -rotateArmDir;
	leftLegAngle += rotateSpeed * -rotateDir;
	rightLegAngle += rotateSpeed * rotateDir;

	if (fabs(leftLegAngle) > 60) {
		// If we've moved further than our threshold allows, reverse direction
		rotateDir *= -1;
	}

	if (fabs(leftArmAngle) > 90) {
		// If we've moved further than our threshold allows, reverse direction
		rotateArmDir *= -1;
	}

	leftArmJoint->getMatrix() = Matrix4::rotX(leftArmAngle) * leftArmMtx; 
	rightArmJoint->getMatrix() = Matrix4::rotX(rightArmAngle) * rightArmMtx;
	leftLegJoint->getMatrix() = Matrix4::rotX(rightLegAngle) * leftLegMtx;
	rightLegJoint->getMatrix() = Matrix4::rotX(leftLegAngle) * rightLegMtx;

}

void Robot::draw(Matrix4& C) {
	
	lastC = (C * *mtx);
	Group::draw( lastC );
	animate();
	
}