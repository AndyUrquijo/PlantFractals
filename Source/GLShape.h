#pragma once

#include <GL\glew.h>

struct GLShapeIndexed
{
	GLuint vertexArray;
	
	GLuint indexBuffer;
	GLuint indexCount;

	GLuint vertexBuffer;
	GLuint vertexCount;

	GLenum primitiveType;


	void Draw( );
};

struct GLShape
{
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint vertexCount;

	GLenum primitiveType;

	void Draw( );
};

struct GLColorShape : GLShapeIndexed
{
	GLuint colorBuffer;
};
