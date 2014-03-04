#include "GLFactory.h"
#include <GL\glew.h>

void GLFactory::CreateVertexBuffer( GLuint index, GLuint compCount, GLenum compType, void* bufferData, GLuint bufferSize )
{

	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_ARRAY_BUFFER, handle );
	glBufferData( GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW );

	glVertexAttribPointer( index, compCount, compType, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( index );
}

void GLFactory::CreateIndexBuffer( void* bufferData, GLuint bufferSize )
{
	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW );
}

