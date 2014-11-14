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
	centerPos = /* *mtx * */ Vector4(0, 0, 0, 1);
	/*x = pos.getX();
	y = pos.getY();
	z = pos.getZ();*/
	x = y = z = 0;
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

	leftArmMtx = Matrix4::translate(-3.5, -2, 0);  //* Matrix4::scale(1, 3, 1);
	rightArmMtx = Matrix4::translate(3.5, -2, 0); //* Matrix4::scale(1, 3, 1);
	leftLegMtx = Matrix4::translate(-1.1, -5, 0);  //* Matrix4::scale(1, 3, 1);
	rightLegMtx = Matrix4::translate(1.1, -5, 0); //* Matrix4::scale(1, 3, 1);

	leftLegJoint = new MatrixTransform( leftLegMtx );
	rightLegJoint = new MatrixTransform( rightLegMtx );
	leftArmJoint = new MatrixTransform( leftArmMtx );
	rightArmJoint = new MatrixTransform( rightArmMtx );
	leftLegScale = new MatrixTransform(Matrix4::scale(1, 3, 1));
	rightLegScale = new MatrixTransform(Matrix4::scale(1, 3, 1));
	leftArmScale = new MatrixTransform(Matrix4::scale(1, 3, 1));
	rightArmScale = new MatrixTransform(Matrix4::scale(1, 3, 1));

	neckJoint = new MatrixTransform(Matrix4::translate(0, 5, 0));
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
	rotateSpeed = .125;
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
	/*
	if ( show == true && drawBoundingSphere != show ) {

		// Add wirespheres

		updateBounds();

		wireHead = new Wiresphere(head->boundingRadius, 10, 10 );
		wireWholeRobot = new Wiresphere(boundingRadius, 10, 10);
		
		wireTorso = new Wiresphere(torso->boundingRadius, 10, 10);
		wireLeftArm = new Wiresphere(leftArm->boundingRadius, 10, 10);
		wireLeftLeg = new Wiresphere(leftLeg->boundingRadius, 10, 10);
		wireRightArm = new Wiresphere(rightArm->boundingRadius, 10, 10);
		wireRightLeg = new Wiresphere(rightLeg->boundingRadius, 10, 10);

		leftLegJoint->addChild(wireLeftLeg);
		rightLegJoint->addChild(wireRightLeg);
		leftArmJoint->addChild(wireLeftArm);
		rightArmJoint->addChild(wireRightArm);
		neckJoint->addChild(wireHead);
		addChild(wireTorso);
		addChild(wireWholeRobot);

	}
	else if ( show == false && drawBoundingSphere != show ) {

		// Remove wirespheres

		removeChild(wireTorso);
		removeChild(wireWholeRobot);
		leftLegJoint->removeChild(wireLeftLeg);
		rightLegJoint->removeChild(wireRightLeg);
		leftArmJoint->removeChild(wireLeftArm);
		rightArmJoint->removeChild(wireRightArm);
		neckJoint->removeChild(wireHead);
		delete wireHead, wireLeftArm, wireLeftLeg, wireRightArm, wireRightLeg, wireTorso;
	}
	*/
	drawBoundingSphere = show;

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