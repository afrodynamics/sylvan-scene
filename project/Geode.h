#pragma once
#include "node.h"
class Geode :
	public Node
{
public:
	Geode();
	~Geode();
	virtual void render() = 0; // Written in subclasses, i.e. Cube and Sphere
	void draw(Matrix4& C);   // Inherited from Node
	void showBoundingBox(bool);
	void setShader(Shader*); // Attach shader to this node
	void enableShader(bool); // Enable/disable shader
};