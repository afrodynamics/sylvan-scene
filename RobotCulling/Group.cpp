#include <list>
#include "Group.h"

using namespace std;

Group::Group()
{
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
	children.emplace_front( node );
};

void Group::removeChild(Node* node) {
	children.remove(node);
};