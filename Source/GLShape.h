#pragma once

#include <GL\glew.h>

struct GLShape
{
	GLuint vertexArray;
	
	GLuint vertexBuffer;
	GLuint indexBuffer;
	
	GLuint indexCount;
	//GLuint vertexCount;

	GLenum primitiveType;


	void Draw( );
};

struct GLSimpleShape
{
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint vertexCount;

	GLenum primitiveType;

	void Draw( );
};