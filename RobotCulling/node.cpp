#include "node.h"

int Node::lightCounter = 0;

Node::Node()
{
	drawBoundingSphere = false;
	culling = false;
}

Node::~Node() {}