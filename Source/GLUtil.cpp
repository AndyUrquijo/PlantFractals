#include "GLUtil.h"


void ZeroGLBuffer(uint bufferSize, GLenum target)
{
	char* dataDest = (char*)glMapBuffer( target, GL_WRITE_ONLY );
	if ( !dataDest ) return WinApp::ShowErrorMessage( L"Error while updating buffer" );
	
	//ZeroMemory(dataDest, bufferSize);
	
	glUnmapBuffer( target );

}