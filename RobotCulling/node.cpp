#include "node.h"


Node::Node()
{
	drawBoundingSphere = false;
}


Node::~Node()
{
}

void Node::showBoundingBox(bool show) {
	drawBoundingSphere = show;
}