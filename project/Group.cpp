#include <list>
#include <iostream>
#include "Group.h"

using namespace std;

Group::Group()
{
	boundingRadius = 0;
	lastC.identity();
}

Group::~Group()
{
}

void Group::draw(Matrix4& C) {
	
	list<Node*>::iterator iter = children.begin();

	// Enable shader, if it exists
	if ( shader != nullptr && useShader ) {
		shader->bind();
	}

	while (iter != children.end()) {
		(*iter)->draw(C);
		iter++;
	}

	// Disbale the shader
	if ( shader != nullptr && useShader ) {
		shader->unbind();
	}

	if (drawBoundingSphere == true) {

		centerPos = lastC * Vector4(0, 0, 0, 1);
		scale = lastC * Vector4(.5, .5, .5, 0);
		boundingRadius = scale.length();
		Matrix4 tmp = Matrix4::translate(centerPos.getX(), centerPos.getY(), centerPos.getZ()) *
			Matrix4::scale(boundingRadius, boundingRadius, boundingRadius);
		tmp.transpose();
		glPushMatrix();
		glLoadMatrixd(tmp.getPointer());
		glutWireSphere(1, 16, 16);
		glPopMatrix();

	}
}

// We aren't going to manage the memory in the list anymore
void Group::addChild(Node* node) {
	if (node == nullptr) {
		throw "Can't add nullptr to a SceneGraph group.";
	}
	children.push_front( node );
};

void Group::removeChild(Node* node) {
	if (node == nullptr) {
		throw "Can't remove nullptr from a SceneGraph group.";
	}
	children.remove(node);
};

void Group::showBoundingBox(bool show) {
	drawBoundingSphere = show;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->showBoundingBox(show);
		iter++;
	}
}

void Group::setShader(Shader* shad) {
	if ( shader != nullptr ) {
		this->shader = shad;
	}
	else {
		this->shader = shad; // Potential memory leak, behavior as yet undecided	
	}
}; // Attach shader to this node
void Group::enableShader(bool toggle) {
	useShader = toggle;
}; // Enable/disable shader