#include "GLShader.h"
#include <GLTools.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLRenderer.h"
#include "WinApp.h"

GLShader::GLShader( )
{
}


GLShader::~GLShader( )
{
}

void GLShader::LoadShader( const char* vsName, const char* fsName, uint loc0, const char* name0 )
{
	shaderHandle = gltLoadShaderPairWithAttributes( vsName, fsName, 1, loc0, name0 );
	if ( !shaderHandle ) WinApp::ShowErrorMessage(L"The shader could not be loaded", L"Error" );
}
void GLShader::LoadShader( const char* vsName, const char* fsName, uint loc0, const char* name0, uint loc1, const char* name1 )
{
	shaderHandle = gltLoadShaderPairWithAttributes( vsName, fsName, 2, loc0, name0, loc1, name1 );
	if ( !shaderHandle ) WinApp::ShowErrorMessage(L"The shader could not be loaded", L"Error" );
}

void GLShader::ObtainUniform( Uniform uniform, const char * name )
{
	uniforms[uniform] = glGetUniformLocation( shaderHandle, name );
	if ( uniforms[uniform] < 0 ) WinApp::ShowErrorMessage(L"One of the uniforms in the shader was not loaded", L"Error" );
}

void GLShader::Use( )
{
	glUseProgram( shaderHandle );
	GLRenderer::SetShader( this );
}
