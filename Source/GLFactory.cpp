#include "GLFactory.h"
#include <GL\glew.h>
#include <GL\gl.h>

GLuint GLFactory::CreateVertexBuffer( GLuint index, GLuint compCount, GLenum compType, void* bufferData, GLuint bufferSize )
{

	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_ARRAY_BUFFER, handle );
	glBufferData( GL_ARRAY_BUFFER, bufferSize, bufferData, GL_DYNAMIC_DRAW );

	glVertexAttribPointer( index, compCount, compType, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( index );

	return handle;
}

GLuint GLFactory::CreateIndexBuffer( void* bufferData, GLuint bufferSize )
{
	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW );
	
	return handle;
}

GLuint GLFactory::CreateShaderStorageBuffer( void* bufferData, GLuint bufferSize )
{
	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_SHADER_STORAGE_BUFFER, handle );
	glBufferData( GL_SHADER_STORAGE_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW );
	
	return handle;
}

