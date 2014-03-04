#pragma once

#include "GLSharedTypes.h"

enum Uniform { WORLD, VP, PL_COLOR, PL_POSITION, NUM_UNIFORMS };


//Wrapper class for Richard Wright's Shader loading functions
class GLShader
{
	uint	shaderHandle;
	uint	uniforms[NUM_UNIFORMS];		//TODO: use a map to store only the needed uniforms

	public:
	GLShader( );
	~GLShader( );

	void LoadShader( const char* vsName, const char* fsName, uint loc0, const char* name0 );
	void LoadShader( const char* vsName, const char* fsName, uint loc0, const char* name0, uint loc1, const char* name1 );
	void ObtainUniform( Uniform uniform, const char * name );
	uint GetUniform( Uniform uniform ) { return uniforms[uniform]; }
	void Use();
};

