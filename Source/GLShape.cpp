#include "GLShape.h"

void GLShape::Draw( )
{
	glBindVertexArray( vertexArray );
	glDrawElements( primitiveType, indexCount, GL_UNSIGNED_INT, 0 );
}

void GLSimpleShape::Draw( )
{
	glBindVertexArray( vertexArray );
	glDrawArrays( primitiveType, 0, vertexCount );
}