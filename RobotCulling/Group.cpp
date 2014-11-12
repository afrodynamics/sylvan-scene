#include <list>
#include <iostream>
#include "Group.h"

using namespace std;

Group::Group()
{
	boundingRadius = 0;
}


Group::~Group()
{
}

void Group::draw(Matrix4& C) {
	list<Node*>::iterator iter = children.begin();
	while (iter != children.end()) {
		(*iter)->draw(C);
		if (drawBoundingSphere) {

			Vector3 objPos = (*iter)->centerPos;
			Vector4 pos = Vector4(objPos.getX(), objPos.getY(), objPos.getZ(), 1);
			if (pos.length() > boundingRadius) {
				boundingRadius = objPos.length();
			}

		}
		iter++;
	}

	// Draw the bounding sphere around this Group
	/*if (drawBoundingSphere) {

		C.transpose();
		glLoadMatrixd(C.getPointer());
		glutWireSphere(boundingRadius, 10, 10);
		C.transpose();

	}*/
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