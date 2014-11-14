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
	while (iter != children.end()) {
		(*iter)->draw(C);
		iter++;
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

void Group::setCulling(bool show) {
	culling = show;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->setCulling(show);
		iter++;
	}
};

void Group::setVector(std::vector<Plane> *f) {
	frustumPlanes = f;
	auto iter = children.begin();
	while (iter != children.end()) {
		(*iter)->setVector(f);
		iter++;
	}
}