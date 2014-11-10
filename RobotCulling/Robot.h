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
	double leftLimbAngle, rightLimbAngle;
	double rotateDir, rotateSpeed;
	Vector3 armAxis, legAxis;
	Matrix4 leftArmMtx, rightArmMtx, leftLegMtx, rightLegMtx;

	// Helper function for constructing the geometry
	void createRobot(void);
public:
	Robot();
	Robot(Matrix4&);
	Robot(double, double, double);
	~Robot();
	void animate();
};

