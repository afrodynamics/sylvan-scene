#pragma once
#include "Group.h"
class MatrixTransform :
	public Group
{
protected:
	Matrix4 *mtx;
public:
	MatrixTransform();
	MatrixTransform(Matrix4&); // Copies the given Matrix4 into this Node
	~MatrixTransform();
	void draw(Matrix4& C);
	Matrix4& getMatrix(); // Returns the matrix stored in this Node
};

