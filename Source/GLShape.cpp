#include "GLShape.h"

void GLShapeIndexed::Draw( )
{
	glBindVertexArray( vertexArray );
	glDrawElements( primitiveType, indexCount, GL_UNSIGNED_INT, 0 );
}

void GLShape::Draw( )
{
	glBindVertexArray( vertexArray );
	glDrawArrays( primitiveType, 0, vertexCount );
}