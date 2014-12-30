#include "GLShader.h"

#include "GLRenderer.h"
#include "WinApp.h"
#include <fstream>
using namespace std;

#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\GL.h>


void GLShader::CreateProgram( )
{
	program = glCreateProgram( );
}

GLuint GLShader::LoadShader( PCSTR filePath, GLenum type )
{
	CHAR filePathFull[30] = "Shaders/";
	strcat_s(filePathFull, sizeof filePathFull, filePath);
	// Read source file

	ifstream ifs( filePathFull, ios_base::in | ios_base::binary );
	if ( !ifs.is_open( ) ) WinApp::ShowErrorMessage( L"Could not open the shader file" );

	ifs.seekg( 0, std::ios::end );
	GLint sourceSize = (GLint)ifs.tellg( );
	ifs.seekg( 0, std::ios::beg );

	GLchar* source = new GLchar[sourceSize];
	ifs.read( source, sourceSize );
	ifs.close( );

	// Create & Compile Shader

	GLuint shaderID = glCreateShader( type );

	glShaderSource( shaderID, 1, (const GLchar**)&source, &sourceSize );
	glCompileShader( shaderID );

	// Verify shader

	GLint isCompiled = 0;
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &isCompiled );
	if ( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &maxLength );

		GLchar* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog( shaderID, maxLength, &maxLength, infoLog );

		wchar_t* winfoLog = new wchar_t[maxLength];
		for ( GLint i = 0; i < maxLength; i++ )
			winfoLog[i] = (wchar_t) infoLog[i];
		WinApp::ShowErrorMessage( winfoLog );

		glDeleteShader( shaderID );
	}
	glAttachShader( program, shaderID );

	return shaderID;
}

void GLShader::BindAttribute( GLuint index, PCSTR attribute )
{
	glBindAttribLocation( program, index, attribute );
}


void GLShader::CompileProgram(  )
{
	glLinkProgram( program );

	GLint numShaders = 0;
	glGetProgramiv( program, GL_ATTACHED_SHADERS, &numShaders );
	GLuint* shaders = new GLuint[numShaders];
	glGetAttachedShaders( program, numShaders, &numShaders, shaders );
	for ( GLint i = 0; i < numShaders; i++ )
		glDeleteShader( shaders[i] );
	delete[] shaders;

	GLint isCompiled = 0;
	glGetProgramiv( program, GL_LINK_STATUS, &isCompiled );
	if ( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		GLchar* infoLog = new GLchar[maxLength];
		glGetProgramInfoLog( program, maxLength, &maxLength, infoLog );

		wchar_t* winfoLog = new wchar_t[maxLength];
		for ( GLint i = 0; i < maxLength; i++ )
			winfoLog[i] = (wchar_t) infoLog[i];
		WinApp::ShowErrorMessage( winfoLog );

		glDeleteProgram( program );
	}
}

void GLShader::ObtainUniform( Uniform uniform, const char * name )
{
	uniforms[uniform] = glGetUniformLocation( program, name );
	if ( uniforms[uniform] < 0 ) WinApp::ShowErrorMessage( L"One of the uniforms in the shader was not loaded", L"Error" );
}

void GLShader::Use( )
{
	glUseProgram( program );
	GLRenderer::SetShader( this );
}
