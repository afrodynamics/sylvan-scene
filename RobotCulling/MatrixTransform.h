#pragma once
#include "Group.h"
class MatrixTransform :
	public Group
{
protected:
	
public:
	Matrix4 *mtx;
	MatrixTransform();
	MatrixTransform(Matrix4&); // Copies the given Matrix4 into this Node
	~MatrixTransform();
	void draw(Matrix4& C);
	void updateBounds();
	void setVector(std::vector<Plane>*);
	Matrix4& getMatrix(); // Returns the matrix stored in this Node
};

