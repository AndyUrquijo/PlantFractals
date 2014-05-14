#pragma once

#include "GLShape.h"

enum GLSpace { GL_LOCAL, GL_WORLD };

#include "Math.h"
#include "GLSharedTypes.h"

/*
Manages information required to draw 3D objects.
*/
class GLObject
{

	Math::Matrix44		localTransform;	// Transform Matrix, does not affect children
	Math::Matrix44		worldTransform;	// Transform Matrix, affects children

	Math::Vector4		color;		// RGBA color
	GLint				texture;	// Handle to the texture

public:
	GLShape				shape;
//	GLShape& GetShape() { return shape; }

	GLObject();


	void Translate(GLSpace space, float x, float y, float z);
	void Scale(GLSpace space, float scale);
	void Scale(GLSpace space, float x, float y, float z);
	//void Rotate(GLSpace space, float angle, float axisX, float axisY, float axisZ);
	void Orbit(GLSpace space, float angle, float axisX, float axisY, float axisZ);

	/*
	Draws the shape using the current matrix information in the stack. 
	If preserveStack is true, it returns it to its previous state after using it.
	*/
	void Draw(bool preserveStack = true);

};