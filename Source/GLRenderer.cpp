
#include "GLRenderer.h"
#include "WinApp.h"

#include "GLGeometry.h"
#include "GLSharedTypes.h"
#include "GLFactory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef DrawText

#include "Clock.h"
#include <sstream>


#include <thread>

GLRenderer GLRenderer::instance;
GLShader* GLRenderer::currShader;


void GLRenderer::Initialize( void )
{
	glewInit( );

	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	plantShader.CreateProgram( );
	plantShader.LoadShader( "Shaders/Plant.vp", GL_VERTEX_SHADER );
	plantShader.LoadShader( "Shaders/Plant.gp", GL_GEOMETRY_SHADER );
	plantShader.LoadShader( "Shaders/Plant.fp", GL_FRAGMENT_SHADER );
	plantShader.BindAttribute( VERTEX_POSITION, "_position" );
	plantShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	plantShader.CompileProgram( );
	plantShader.ObtainUniform( WORLD, "WORLD" );
	plantShader.ObtainUniform( VP, "VP" );

	plantComputeShader.CreateProgram( );
	plantComputeShader.LoadShader( "Shaders/Plant.cp", GL_COMPUTE_SHADER );
	plantComputeShader.CompileProgram( );
	plantComputeShader.ObtainUniform( TIME, "TIME" );

	camera.position = { 0, 10, 0 };

	InitializeObjects( );

	GLText::InitializeText( );
	text.LoadFont( "Book Antiqua Bold 24.bff" );
	text.AddText( L"Testy123", { 0.1f, 0.1f } );
	text.AddText( L"Hola", { 0.15f, 0.05f } );

	/* //Simulate work
	for ( size_t i = 0; i < 3; i++ )
	( new std::thread( []{ while ( true ); } ) )->detach( );
	*/
}



void GLRenderer::InitializeObjects( void )
{
	Plant::InitializeSystem( );
}

///////////////////////////////////////////////////////////////////////////////
// Do your cleanup here. Free textures, display lists, buffer objects, etc.
void GLRenderer::Terminate( void )
{
}


///////////////////////////////////////////////////////////////////////////////
// This is called at least once and before any rendering occurs. If the screen
// is a resizeable window, then this will also get called whenever the window
// is resized.
void GLRenderer::Resize( GLsizei nWidth, GLsizei nHeight )
{
	glViewport( 0, 0, nWidth, nHeight );

	projMatrix = Matrix44::MakeProjectionMatrix( 0.333f*PI, float( nWidth ) / float( nHeight ), 1.0f, 1000.0f );
}


///////////////////////////////////////////////////////////////////////////////
// Render a frame. The owning framework is responsible for buffer swaps,
// flushes, etc.
void GLRenderer::Render( void )
{
	text.ClearText( );

	static Clock clock;
	static int counter = 0;
	static float timer = 0;
	static float lastMark = 0;
	static float fps = 0;
	timer = (float) clock.Watch( );
	counter++;
	if ( counter == 10 )
	{
		fps = 10 / ( timer - lastMark );
		counter = 0;
		lastMark = timer;
	}


	std::wstringstream ss;
	ss << L"FPS: " << fps;
	text.AddText( ss.str( ), { -0.95f, +0.95f } );

	ss.str( L"" );
	ss << "Camera Pos: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.90f } );

	ss.str( L"" );
	ss << "Camera Vel: (" << camera.velocity.x << ", " << camera.velocity.y << ", " << camera.velocity.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.85f } );

	ss.str( L"" );
	ss << "Camera Ang: (" << camera.orientation.x*( 180 / 3.14f ) << ", " << camera.orientation.y*( 180 / 3.14f ) << ", " << camera.orientation.z*( 180 / 3.14f ) << ")";
	text.AddText( ss.str( ), { -0.95f, +0.80f } );

	wglMakeCurrent( WinApp::deviceContext, WinApp::renderContext );

	if ( GetAsyncKeyState( 'P' ) )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );



	camera.MoveCamera( );

	Math::Matrix44 viewProjection = camera.MakeViewMatrix( )*projMatrix;
	

	static Clock plantClock;
	Plant::timeVal = plantClock.Watch( );

#define USE_COMPUTE

#ifdef USE_COMPUTE
	plantComputeShader.Use( );
	glUniform1f( plantComputeShader.GetUniform( TIME ), Plant::timeVal );

	for ( UINT i = 0; i < Plant::plantArray.size( ); i++ )
		Plant::plantArray[i].UpdateWithCompute( );

#endif


	// Plant Draws
	{
		glEnable( GL_DEPTH_TEST );
		glClear( GL_COLOR_BUFFER_BIT );
		glClear( GL_DEPTH_BUFFER_BIT );


		plantShader.Use( );
		glUniformMatrix4fv( plantShader.GetUniform( VP ), 1, GL_FALSE, viewProjection.elm );




		for ( UINT i = 0; i < Plant::plantArray.size( ); i++ )
		{
#ifndef USE_COMPUTE
			Plant::plantArray[i].Update( );
#endif
			Plant::plantArray[i].UpdateObject( );

		}
		for ( UINT i = 0; i < Plant::plantArray.size( ); i++ )
		{
			Plant::plantArray[i].Draw( );
		}
	}


	glDisable( GL_DEPTH_TEST);
	text.DrawText( );

	BOOL result = SwapBuffers( WinApp::deviceContext );
	glFinish( );
	wglMakeCurrent( NULL, NULL );
}

