#pragma once
#include <list>
#include "node.h"
class Group :
	public Node
{
protected:
	std::list<Node*> children;
public:
	Group();
	~Group();

	void addChild(Node*); // Takes a const Node reference, copies it and allocates it with new
	void removeChild(Node*); 
	void draw(Matrix4& C);
};

