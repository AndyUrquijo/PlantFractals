#pragma once

#include <GL\glew.h>

struct GLShape
{
	GLuint vertexArray;
	GLuint size;
	bool indexed;

	GLenum primitiveType;

	void Draw( );
};

