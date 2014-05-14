#include "GLObject.h"
#include "GLRenderer.h"
//#include <math3d.h>

#include "Math.h"
using namespace Math;

GLObject::GLObject( )
{
	localTransform = Matrix44::MakeIdentity( );
	worldTransform = Matrix44::MakeIdentity( );

	texture = -1;
}

void GLObject::Draw( bool preserveStack )
{

	GLRenderer& renderer = GLRenderer::GetInstance( );
	MatrixStack& stack = renderer.GetMatrixStack( );

	if ( preserveStack )
		stack.PushMatrix( );
	stack.MultMatrix( worldTransform.elm );

	Matrix44 worldMatrix = stack.GetMatrix( );

	worldMatrix = worldMatrix*localTransform;

	glUniformMatrix4fv( GLRenderer::GetShader().GetUniform(WORLD), 1, GL_FALSE, worldMatrix.elm );
	if ( texture >= 0 )
		glBindTexture( GL_TEXTURE_2D, texture );

	shape.Draw( );

	if ( preserveStack )
		stack.PopMatrix( );

}

void GLObject::Translate( GLSpace space, float x, float y, float z )
{
	Matrix44 translate = Matrix44::MakeTranslation( { x, y, z } );
	switch ( space ) 	{
	case GL_LOCAL: localTransform = translate*localTransform; return;
	case GL_WORLD: worldTransform = translate*worldTransform; return;
	}
}

void GLObject::Scale( GLSpace space, float ratio )
{
	Matrix44 scale = Matrix44::MakeIdentity()*ratio;
	switch ( space ) 	{
	case GL_LOCAL: localTransform = scale*localTransform; return;
	case GL_WORLD: worldTransform = scale*worldTransform; return;
	}
}

void GLObject::Scale( GLSpace space, float x, float y, float z )
{
	Matrix44 scale = Matrix44::MakeIdentity();
	scale.m[0][0] *= x;
	scale.m[1][1] *= y;
	scale.m[2][2] *= z;
	switch ( space ) 	{
	case GL_LOCAL: localTransform = scale*localTransform; return;
	case GL_WORLD: worldTransform = scale*worldTransform; return;
	}
}

//void GLObject::Rotate( GLSpace space, float angle, float axisX, float axisY, float axisZ )
//{
//	Matrix44 rotate = Matrix44::MakeYRotation(angle)*MAtrk
//	m3dRotationMatrix44( rotate.elm, angle, axisX, axisY, axisZ );
//	switch ( space ) 	{
//	case GL_LOCAL: localTransform = rotate*localTransform; return;
//	case GL_WORLD: worldTransform = rotate*worldTransform; return;
//	}
//}

//void GLObject::Orbit( GLSpace space, float angle, float axisX, float axisY, float axisZ )
//{
//	Matrix44 orbit;
//	m3dRotationMatrix44( orbit.elm, angle, axisX, axisY, axisZ );
//	switch ( space ) 	{
//	case GL_LOCAL: localTransform = orbit*localTransform; return;
//	case GL_WORLD: worldTransform = orbit*worldTransform; return;
//	}
//
//}