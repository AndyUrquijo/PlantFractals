
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

	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );
	
	InitializeObjects( );

	GLText::InitializeText( );
	text.LoadFont( "Book Antiqua Bold 24.bff" );
	text.AddText( L"Testy123", { 0.1f, 0.1f } );
	text.AddText( L"Hola", { 0.15f, 0.05f } );


	generalShader.CreateProgram( );
	generalShader.LoadShader( "Plain.vp", GL_VERTEX_SHADER );
	generalShader.LoadShader( "Plain.fp", GL_FRAGMENT_SHADER );
	generalShader.BindAttribute( VERTEX_POSITION, "_position" );
	generalShader.CompileProgram( );
	generalShader.ObtainUniform( VP, "VP" );

	/* //Simulate work
	for ( size_t i = 0; i < 3; i++ )
	( new std::thread( []{ while ( true ); } ) )->detach( );
	*/
}



void GLRenderer::InitializeObjects( void )
{
	plantSystem.Initialize( );

	GLGeometry::QUAD_DESC quadDesc = { { 350, 350 } };
	planeShape = GLGeometry::MakeQuad( quadDesc );

}

void GLRenderer::Terminate( void )
{
}


void GLRenderer::Resize( GLsizei nWidth, GLsizei nHeight )
{
	glViewport( 0, 0, nWidth, nHeight );

	projMatrix = Matrix44::MakeProjectionMatrix( 0.333f*PI, float( nWidth ) / float( nHeight ), 1.0f, 1000.0f );
}


void GLRenderer::Render( void )
{
	UpdateText( );



	wglMakeCurrent( WinApp::deviceContext, WinApp::renderContext );

	if ( GetAsyncKeyState( 'P' ) )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


	camera.MoveCamera( );

	viewProjection = camera.MakeViewMatrix( )*projMatrix;



	//Set blending
	//glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Set antialiasing/multisampling
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	//glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );
	glEnable( GL_MULTISAMPLE );

	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );


	
	plantSystem.Update( );
	plantSystem.Render( );
	
	
	generalShader.Use( );
	glBindBuffer( GL_ARRAY_BUFFER, planeShape.vertexBuffer );
	glVertexAttribPointer( VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glUniformMatrix4fv( GetShader( ).GetUniform( VP ), 1, GL_FALSE, viewProjection.elm );
	planeShape.Draw( );
	

	text.DrawText( );

	BOOL result = SwapBuffers( WinApp::deviceContext );
	glFinish( );
	wglMakeCurrent( NULL, NULL );
}

void GLRenderer::UpdateText( )
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

	const Vector3 camPos = camera.GetPosition(); 
	ss.str( L"" );
	ss << "Camera Pos: (" << camPos.x << ", " << camPos.y << ", " << camPos.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.90f } );

	ss.str( L"" );
	ss << "Camera Vel: (" << camera.velocity.x << ", " << camera.velocity.y << ", " << camera.velocity.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.85f } );

	const Vector3 camFwd = camera.GetForward(); 
	ss.str( L"" );
	ss << "Camera Fwd: (" << camFwd.x << ", " << camFwd.y << ", " << camFwd.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.80f } );
}


