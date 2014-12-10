#include <vector>
#include <cmath>
#include <iostream>
#include "Geode.h"

using namespace std;

// Constructor
Geode::Geode()
{
	drawBoundingSphere = false;
	scale = Vector4(0, 0, 0, 1);
	centerPos = Vector4(0, 0, 0, 1);
}

// Nothing to destroy
Geode::~Geode()
{
}

// Polymorphic draw method, updates lastC and sets the modelview
// matrix in OpenGL. Assumes C is a row major matrix
void Geode::draw(Matrix4& C) {
	
	Matrix4 tmp = C;
	lastC = C;
	centerPos = C * Vector4(0,0,0,1);

	// If we aren't culling, just pass in everything to OpenGL
	tmp.transpose();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( tmp.getPointer() );

	if ( useShader && shader != nullptr ) {
		shader->bind();
	}
  mat.apply();        // Apply the Material
	render();
	if ( useShader && shader != nullptr ) {
		shader->unbind();
	}

}

// Toggles showing the bounding sphere around this object
void Geode::showBoundingBox(bool show) {
	this->drawBoundingSphere = show;
}

// Toggle shader
void Geode::enableShader(bool shad) {
	useShader = shad;
}

void Geode::setShader(Shader* shad) {
	shader = shad;
}

// Set the Material for the Geode
void Geode::setMat(Material m) {
	mat = m;
}