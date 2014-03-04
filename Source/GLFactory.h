#pragma once

#include "GLSharedTypes.h"

namespace GLFactory
{
	// Creates a buffer using the supplied information.
	// Assumes the corresponding vertex array object is bound.
	void CreateVertexBuffer( GLuint index, GLuint compCount, GLenum compType, void* bufferData, GLuint bufferSize );

	// Creates an index buffer using the supplied information.
	// Assumes the corresponding vertex array object is bound.
	void CreateIndexBuffer( void* bufferData, GLuint bufferSize );
};

