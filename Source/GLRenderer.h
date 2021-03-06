/*		GLRenderer.h		by Andres Urquijo		*/

#pragma once

#include <stdio.h>
#include <GL\glew.h>

#include "Camera.h"
#include "Plant.h"
#include "GLSharedTypes.h"
#include "MatrixStack.h"
#include "GLShader.h"
#include "GLText.h"
#include "PlantSystem.h"
#include "GLUtil.h"

//OpenGL Renderer
class GLRenderer
{
	protected:
	static GLRenderer instance;
	static GLShader* currShader;

	MatrixStack			matrixStack;
	Camera				camera;
	Matrix44			projMatrix;
	Matrix44			viewProjection;

	GLShape		planeShape;
	GLShader			generalShader;

	GLText				text;
	PlantSystem			plantSystem;

	GLShader			testCompute;
	GLShader			testDraw;
	GLuint				testBuffer;
	GLColorShape		testShape;
	GLushort			testCols;
	GLushort			testRows;

	public:
	virtual ~GLRenderer( ) {};

	void Initialize( void );							// Called after context is created
	void InitializeObjects( void );
	void Terminate( void );								// Called before context is destroyed
	void Resize( GLsizei nWidth, GLsizei nHeight );		// Called when window is resized, at least once when window is created
	void Render( void );								// Called to update OpenGL view

	void UpdateText( );

	inline static GLRenderer&	GetInstance( )					{ return instance; }
	inline static GLShader&		GetShader( )					{ return *currShader; }
	
	inline static Matrix44&		GetProjectionMatrix( )			{ return instance.projMatrix; }
	inline static Matrix44&		GetViewProjectionMatrix( )		{ return instance.viewProjection; }
	inline static MatrixStack&	GetMatrixStack( )				{ return instance.matrixStack; }
	inline static Vector3		GetCameraPos( )					{ return instance.camera.GetPosition(); }

	inline static void			SetShader( GLShader* shader )	{ currShader = shader; }


};




