#pragma once

#include <GL\glew.h>
#include "GLSharedTypes.h"
#include "Winapp.h"

template<class TYPE>
void UpdateGLBuffer(TYPE* data, uint elmCount, GLenum target)
{
	TYPE* dataDest = (TYPE*) glMapBuffer( target, GL_WRITE_ONLY );
	if ( !dataDest ) return WinApp::ShowErrorMessage( L"Error while updating buffer" );

	memcpy( dataDest, data, elmCount*sizeof(TYPE) );

	glUnmapBuffer( target );

}

void ZeroGLBuffer(uint bufferSize, GLenum target);
