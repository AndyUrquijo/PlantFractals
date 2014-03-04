#include "GLShape.h"

void GLShape::Draw( )
{
	glBindVertexArray( vertexArray );
	if ( indexed )
		glDrawElements( primitiveType, size, GL_UNSIGNED_SHORT, 0 );
	else
		glDrawArrays( primitiveType, 0, size );
}