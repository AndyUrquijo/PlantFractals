
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

//#define COMPUTE_TEST

void GLRenderer::Initialize( void )
{
	glewInit( );

	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );
	//glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );


	camera.position = { 0, -15, 30 };

	InitializeObjects( );

	GLText::InitializeText( );
	text.LoadFont( "Book Antiqua Bold 24.bff" );
	text.AddText( L"Testy123", { 0.1f, 0.1f } );
	text.AddText( L"Hola", { 0.15f, 0.05f } );


	generalShader.CreateProgram( );
	generalShader.LoadShader( "Shaders/Plain.vp", GL_VERTEX_SHADER );
	generalShader.LoadShader( "Shaders/Plain.fp", GL_FRAGMENT_SHADER );
	generalShader.BindAttribute( VERTEX_POSITION, "_position" );
	generalShader.CompileProgram( );
	generalShader.ObtainUniform( VP, "VP" );

#ifdef COMPUTE_TEST
	testCompute.CreateProgram( );
	testCompute.LoadShader( "Shaders/Test.cp", GL_COMPUTE_SHADER );
	testCompute.CompileProgram( );

	testDraw.CreateProgram( );
	testDraw.LoadShader( "Shaders/Test.vp", GL_VERTEX_SHADER );
	testDraw.LoadShader( "Shaders/Test.fp", GL_FRAGMENT_SHADER );
	testDraw.BindAttribute( VERTEX_POSITION, "_position" );
	testDraw.BindAttribute( VERTEX_COLOR, "_color" );
	testDraw.CompileProgram( );
#endif
	/* //Simulate work
	for ( size_t i = 0; i < 3; i++ )
	( new std::thread( []{ while ( true ); } ) )->detach( );
	*/
}



void GLRenderer::InitializeObjects( void )
{
	plantSystem.Initialize( );

	GLGeometry::QUAD_DESC quadDesc = { { 500, 500 } };
	planeShape = GLGeometry::MakeQuad( quadDesc );

#ifdef COMPUTE_TEST
	testCols = 30;
	testRows = 30;
	
	GLGeometry::PLANE_GRID_DESC planeGridDesc;
	planeGridDesc.layers = { (float) testCols, (float) testRows };
	planeGridDesc.size = { 2, 2 };
	planeGridDesc.center = { 0, 0, 0 };

	testShape = GLGeometry::MakePlaneGrid( planeGridDesc );
	testBuffer = testShape.vertexBuffer;

	glBindBuffer(GL_ARRAY_BUFFER, testShape.colorBuffer );

	Vector4* colors = (Vector4*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	if ( !colors ) return WinApp::ShowErrorMessage( L"Couldn't read the test buffer" );
		
	colors[(testCols+1) / 2 + (testCols+1)*((testRows+1) / 2)] = { 1, 1, 1, 1 };

	glUnmapBuffer( GL_ARRAY_BUFFER );
#endif
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


	#ifdef COMPUTE_TEST
	
	testCompute.Use( );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, testShape.colorBuffer );

	glDispatchCompute( testCols+1, testRows+1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	

	glDisable( GL_DEPTH_TEST );
	testDraw.Use();
	testShape.Draw( );

	//Sleep(800);

#else
	
	plantSystem.Update( );
	plantSystem.Render( );
	

	
	generalShader.Use( );
	glBindBuffer( GL_ARRAY_BUFFER, planeShape.vertexBuffer );
	glVertexAttribPointer( VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glUniformMatrix4fv( generalShader.GetUniform( VP ), 1, GL_FALSE, viewProjection.elm );
	planeShape.Draw( );
	
#endif

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

	ss.str( L"" );
	ss << "Camera Pos: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.90f } );

	ss.str( L"" );
	ss << "Camera Vel: (" << camera.velocity.x << ", " << camera.velocity.y << ", " << camera.velocity.z << ")";
	text.AddText( ss.str( ), { -0.95f, +0.85f } );

	ss.str( L"" );
	ss << "Camera Ang: (" << camera.orientation.x*( 180 / 3.14f ) << ", " << camera.orientation.y*( 180 / 3.14f ) << ", " << camera.orientation.z*( 180 / 3.14f ) << ")";
	text.AddText( ss.str( ), { -0.95f, +0.80f } );
}
