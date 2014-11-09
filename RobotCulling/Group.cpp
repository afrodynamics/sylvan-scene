#include "Group.h"

using namespace std;

Group::Group()
{
}


Group::~Group()
{
	// Deallocate all your children using C++11's awesome iterator type inference
	for (auto iter = children.begin(); iter != children.end(); iter++){
		delete *iter;
	}
}

void Group::draw(Matrix4& C) {
	list<Node*>::iterator iter = children.begin();
	while (iter != children.end()) {
		(*iter)->draw(C);
		iter++;
	}
}