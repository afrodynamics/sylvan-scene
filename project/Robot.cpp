#include <iostream>
#include "Robot.h"

using namespace std;

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
	centerPos = *mtx * Vector4(0, 0, 0, 1);
	x = y = z = 0;
	x = centerPos.getX();
	y = centerPos.getY();
	z = centerPos.getZ();
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
	delete leftLegJoint, leftLegScale, rightLegJoint, rightLegScale;
	delete leftArmJoint, leftArmScale, rightArmJoint, rightArmScale;
	if (wireHead != nullptr) {
		delete wireHead, wireLeftArm, wireLeftLeg, wireRightArm, wireRightLeg, wireTorso;
	}
}

/**
 * Creates the geometry required for the robot
 * and updates the appropriate pointers.
 */
void Robot::createRobot() {

	leftArmMtx = Matrix4::translate(-3.5, -2, 0); 
	rightArmMtx = Matrix4::translate(3.5, -2, 0);
	leftLegMtx = Matrix4::translate(-1.1, -5, 0); 
	rightLegMtx = Matrix4::translate(1.1, -5, 0);
	neckMtx = Matrix4::translate(0, 5, 0);

	leftLegJoint = new MatrixTransform( leftLegMtx );
	rightLegJoint = new MatrixTransform( rightLegMtx );
	leftArmJoint = new MatrixTransform( leftArmMtx );
	rightArmJoint = new MatrixTransform( rightArmMtx );

	Matrix4 limbScale = Matrix4::scale(1, 3, 1);

	leftLegScale = new MatrixTransform(limbScale);
	rightLegScale = new MatrixTransform(limbScale);
	leftArmScale = new MatrixTransform(limbScale);
	rightArmScale = new MatrixTransform(limbScale);

	neckJoint = new MatrixTransform(neckMtx);
	leftLeg = new Cube(); // len 2
	rightLeg = new Cube(); // len 2
	rightArm = new Cube(); // len 2
	leftArm = new Cube(); // len 2
	torso = new Cube(); // len 5
	head = new Sphere(); // radius 2.5

	Matrix4 legScale = Matrix4::scale(2, 2, 2);
	Matrix4 armScale = Matrix4::scale(2, 2, 2);
	Matrix4 headScale = Matrix4::scale(2.5, 2.5, 2.5);
	Matrix4 torsoScale = Matrix4::scale(5, 5, 5);

	// Add the joints to the Robot (which is basically just a MatrixTransform)
	// We're just calling addChild on the top level of the object's local tree,
	// which in this case is just the 'this' pointer

	MatrixTransform *legUnifScaleLeft, *legUnifScaleRight, *armUnifScaleLeft, *armUnifScaleRight, *torsoUnifScale, *headUnifScale;

	legUnifScaleLeft = new MatrixTransform(legScale);
	legUnifScaleRight = new MatrixTransform(legScale);
	armUnifScaleLeft = new MatrixTransform(armScale);
	armUnifScaleRight = new MatrixTransform(armScale);
	torsoUnifScale = new MatrixTransform(torsoScale);
	headUnifScale = new MatrixTransform(headScale);
	
	addChild(leftLegJoint);
	addChild(leftArmJoint);
	addChild(rightLegJoint);
	addChild(rightArmJoint);
	addChild(neckJoint);
	addChild(torsoUnifScale);
	addChild(headUnifScale);

	leftLegJoint->addChild(leftLegScale);
	leftLegScale->addChild(legUnifScaleLeft);
	legUnifScaleLeft->addChild(leftLeg);

	leftArmJoint->addChild(leftArmScale);
	leftArmScale->addChild(armUnifScaleLeft);
	armUnifScaleLeft->addChild(leftArm);

	rightArmJoint->addChild(rightArmScale);
	rightArmScale->addChild(armUnifScaleRight);
	armUnifScaleRight->addChild(rightArm);

	rightLegJoint->addChild(legUnifScaleRight);
	legUnifScaleRight->addChild(rightLegScale);
	rightLegScale->addChild(rightLeg);

	torsoUnifScale->addChild(torso);

	neckJoint->addChild(headUnifScale);
	headUnifScale->addChild(head);

	// Initialize the animation state
	rotateDir = rotateArmDir = -1;
	rotateSpeed = .3;
	leftArmAngle = 90;
	rightArmAngle = 270;
	leftLegAngle = rightLegAngle = 0;

}

void Robot::showBoundingBox(bool show) {
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->showBoundingBox(show);
		iter++;
	}
	drawBoundingSphere = show;
}

/**
 * Called by displayCallback to update the
 * position of this robot's limbs, etc.
 */
void Robot::animate() {

	leftArmAngle += rotateSpeed * rotateArmDir;
	rightArmAngle += rotateSpeed * -rotateArmDir;
	leftLegAngle += rotateSpeed * -rotateDir;
	rightLegAngle += rotateSpeed * rotateDir;

	if (ABS(leftLegAngle) > 60) {
		// If we've moved further than our threshold allows, reverse direction
		rotateDir *= -1;
	}

	if (ABS(leftArmAngle) > 90) {
		// If we've moved further than our threshold allows, reverse direction
		rotateArmDir *= -1;
	}

	leftArmJoint->getMatrix() = Matrix4::rotX(leftArmAngle) * leftArmMtx; 
	rightArmJoint->getMatrix() = Matrix4::rotX(rightArmAngle) * rightArmMtx;
	leftLegJoint->getMatrix() = Matrix4::rotX(rightLegAngle) * leftLegMtx;
	rightLegJoint->getMatrix() = Matrix4::rotX(leftLegAngle) * rightLegMtx;

}

void Robot::draw(Matrix4& C) {
	
	lastC = C * *mtx;
	centerPos = lastC * Vector4(0, 0, 0, 1);
	Group::draw(lastC);
	animate();
	
}