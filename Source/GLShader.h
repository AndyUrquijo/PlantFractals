#pragma once

#include "GLSharedTypes.h"


enum Uniform { WORLD, VP, TIME, NUM_UNIFORMS };

//Wrapper class for Richard Wright's Shader loading functions
class GLShader
{
	GLuint	program;
	GLint	uniforms[NUM_UNIFORMS];		//TODO: use a map to store only the needed uniforms
	public:


	void CreateProgram();
	GLuint LoadShader( PCSTR fileName, GLenum type );
	void BindAttribute(  GLuint index, PCSTR attribute );
	void CompileProgram( );

	void ObtainUniform( Uniform uniform, const char * name );
	uint GetUniform( Uniform uniform ) { return uniforms[uniform]; }
	void Use( );
};

