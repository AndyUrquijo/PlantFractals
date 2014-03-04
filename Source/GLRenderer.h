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

//OpenGL Renderer
class GLRenderer
{
	GLRenderer( );

	public:

	virtual ~GLRenderer( ) {};

	void Initialize( void );							// Called after context is created
	void InitializeObjects( void );
	void Terminate( void );							// Called before context is destroyed
	void Resize( GLsizei nWidth, GLsizei nHeight );	// Called when window is resized, at least once when window is created
	void Render( void );								// Called to update OpenGL view

	// These methods are used by the calling framework. Set the appropriate internal
	// protected variables to tell the parent framework your desired configuration
	inline static GLRenderer&	GetInstance( )		{ return instance; }
	inline static GLShader&	GetShader( )		{ return *currShader; }
	inline static void		SetShader( GLShader* shader ) { currShader = shader; }
	
	inline GLuint			GetWidth( void )			{ return screenWidth; }
	inline GLuint			GetHeight( void )			{ return screenHeight; }
	inline GLboolean		GetFullScreen( void )		{ return bFullScreen; }

	inline Matrix44&		GetProjectionMatrix( )	{ return projMatrix; }
	inline MatrixStack&		GetMatrixStack( )		{ return matrixStack; }


	protected:
	static GLRenderer instance;
	static GLShader* currShader;

	bool initialized;

	GLsizei	 screenWidth;			// Desired window or desktop width
	GLsizei  screenHeight;			// Desired window or desktop height

	GLboolean bFullScreen;			// Request to run full screen

	MatrixStack			matrixStack;
	Camera				camera;
	Matrix44			projMatrix;


	//Shaders

	GLShader			groundShader;
	GLShader			plantShader;

	GLObject			cubeObj;

	Plant*				plants;
	GLuint				numPlants;

	GLText				text;

};

