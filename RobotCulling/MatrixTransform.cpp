#include "MatrixTransform.h"


MatrixTransform::MatrixTransform()
{
	mtx = new Matrix4();
	mtx->identity();
}

MatrixTransform::MatrixTransform(Matrix4& matrix)
{
	mtx = new Matrix4(matrix);
}

MatrixTransform::~MatrixTransform()
{
	delete mtx;
}

void MatrixTransform::draw(Matrix4& C) {
	Group::draw( C * *mtx );
}

Matrix4& MatrixTransform::getMatrix() {
	return *mtx;
}