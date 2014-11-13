#pragma once
#include "MatrixTransform.h"
#include "Cube.h"
#include "Sphere.h"

class Robot :
	public MatrixTransform
{
private:
	// Pointers to the relelvant child nodes
	MatrixTransform *leftLegJoint, *rightLegJoint, *neckJoint, *leftArmJoint, *rightArmJoint;
	Cube *leftLeg, *rightLeg, *leftArm, *rightArm, *torso;
	Sphere *head;

	// Position/Animation state
	double x, y, z;
	double leftArmAngle, rightArmAngle, leftLegAngle, rightLegAngle;
	double rotateDir, rotateArmDir, rotateSpeed;
	Matrix4 leftArmMtx, rightArmMtx, leftLegMtx, rightLegMtx;

	// Helper function for constructing/modifying geometry
	void createRobot(void);
	void animate();

public:
	Robot();
	Robot(Matrix4&);
	Robot(double, double, double);
	~Robot();
	void draw(Matrix4& C);
};

