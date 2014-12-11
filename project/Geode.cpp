#include <vector>
#include <cmath>
#include <iostream>
#include "Geode.h"

using namespace std;

// Constructor
Geode::Geode()
{
	drawBoundingSphere = useShader = false;
	useMaterial = true; // Use materials by default
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

	// Shaders & Materials don't work well together
	if ( useShader && shader != nullptr ) {
		glDisable(GL_COLOR_MATERIAL);
		shader->bind();
	}
    else if ( useMaterial == true ) {
    	// Apply the Material if we want to
    	mat.apply();   
    }
    else {
    	defaultMat.apply();
    }

	render();

	if ( useShader && shader != nullptr ) {
		shader->unbind();
	}
	else {
		defaultMat.apply();
	}

}

// Toggles showing the bounding sphere around this object
void Geode::showBoundingBox(bool show) {
	this->drawBoundingSphere = show;
}

// Toggle shader
void Geode::enableShader(bool shad) {
	useShader = shad;
	if ( shad == true && useMaterial ) {
		useMaterial = false; // Disable materials if we're using a shader
	}
}

void Geode::setShader(Shader* shad) {
	shader = shad;
}

// Set the Material for the Geode
void Geode::setMat(Material m) {
	mat = m;
}

// Toggle material
void Geode::enableMat(bool mat) {
	useMaterial = mat;
	if ( mat == true && useShader ) {
		useShader = false; // Disable shaders if we're using a material instead
	}
}