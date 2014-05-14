#pragma once

#include "GLSharedTypes.h"

namespace GLFactory
{
	// Creates a buffer using the supplied information.
	// Assumes the corresponding vertex array object is bound.
	GLuint CreateVertexBuffer( GLuint index, GLuint compCount, GLenum compType, void* bufferData, GLuint bufferSize );

	// Creates an index buffer using the supplied information.
	// Assumes the corresponding vertex array object is bound.
	GLuint CreateIndexBuffer( void* bufferData, GLuint bufferSize );

	GLuint CreateShaderStorageBuffer( void* bufferData, GLuint bufferSize );
};

