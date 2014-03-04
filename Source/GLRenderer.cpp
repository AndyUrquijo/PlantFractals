
#include "GLRenderer.h"
#include "WinApp.h"

#include "GLGeometry.h"
#include "GLSharedTypes.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef DrawText


GLRenderer GLRenderer::instance;
GLShader* GLRenderer::currShader;

GLRenderer::GLRenderer( void ) : screenWidth( 800 ), screenHeight( 600 ), bFullScreen( false ), initialized(false)
{
}

void GLRenderer::Initialize( void )
{
	glewInit( );

	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	groundShader.LoadShader( "Shaders/ColorShader.vp", "Shaders/ColorShader.fp",
							 VERTEX_POSITION, "_position",
							 VERTEX_COLOR, "_color" );
	groundShader.ObtainUniform( WORLD, "WORLD" );
	groundShader.ObtainUniform( VP, "VP" );


	plantShader.LoadShader( "Shaders/Plant.vp", "Shaders/Plant.fp",
							VERTEX_POSITION, "_position" );
	plantShader.ObtainUniform( WORLD, "WORLD" );
	plantShader.ObtainUniform( VP, "VP" );


	//camera.Translate( { 0, 0, 60 } );

	InitializeObjects();

	GLText::InitializeText( );
	text.LoadFont("Book Antiqua Bold 24.bff");
	text.AddText( L"Testy123", { 0.1f, 0.1f } );
	text.AddText( L"Hola", { 0.15f, 0.05f } );

	initialized = true;
}

void GLRenderer::InitializeObjects( void )
{
	GLGeometry::CUBE_DESC cubeDesc;
	cubeDesc.dimensions = { 1, 1, 1 };
	cubeObj.shape = GLGeometry::MakeCube( cubeDesc );
	cubeObj.Translate( GL_WORLD, 0, 3, 0 );


	numPlants = 70;
	plants = new Plant[numPlants];
	for ( UINT i = 0; i < numPlants; i++ )
	{
		plants[i].location.x = Math::RangeRand( -100, 100 );
		plants[i].location.z = Math::RangeRand( -100, 100 );
		plants[i].location.y = 0;
		plants[i].Grow( );
		plants[i].InitializeObject( );
	}
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

	// This is a good place to recompute your projection matrix.
	projMatrix = Matrix44::MakePerspectiveMatrix( 0.333f*PI, float( nWidth ) / float( nHeight ), 1.0f, 1000.0f );
	//m3dMakePerspectiveMatrix( projMatrix, m3dDegToRad( 60.0f ), float( nWidth ) / float( nHeight ), 1.0f, 1000.0f );
}


///////////////////////////////////////////////////////////////////////////////
// Render a frame. The owning framework is responsible for buffer swaps,
// flushes, etc.
void GLRenderer::Render( void )
{
	if( !initialized ) 
		return;

	wglMakeCurrent( WinApp::deviceContext, WinApp::renderContext );
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	matrixStack.PushMatrix( );

	camera.MoveCamera( );

	Math::Matrix44 viewProjection = camera.MakeViewMatrix( ).Inverse( )*projMatrix;
	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );




	groundShader.Use( );
	glUniformMatrix4fv( groundShader.GetUniform( VP ), 1, GL_FALSE, viewProjection.elm );

	cubeObj.Draw( );


	plantShader.Use( );
	glUniformMatrix4fv( plantShader.GetUniform( VP ), 1, GL_FALSE, viewProjection.elm );

	for ( UINT i = 0; i < numPlants; i++ )
		plants[i].Draw( );

	matrixStack.PopMatrix( );


	text.DrawText();

	BOOL result = SwapBuffers( WinApp::deviceContext );
	wglMakeCurrent( NULL, NULL );
}

