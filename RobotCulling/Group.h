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

	// Because I don't want bad things to happen, I'm going to make
	// the Group class copy all the nodes passed into it, so that it's
	// safe to pass in temporaries

	void addChild(const Node&); // Takes a const Node reference, copies it and allocates it with new
	void removeChild(const Node&); 
	void draw(Matrix4& C);
};

