#include "PlantSystem.h"
#include "Clock.h"
#include "GLRenderer.h"
#include "GLFactory.h"
#include <Windows.h>
#include <string>
using std::string;
#include "WinApp.h"

#define PLANT_AMOUNT 50

void PlantSystem::Initialize( )
{
	// Intialize Shaders

	drawShader.CreateProgram( );
	drawShader.LoadShader( "Shaders/Plant.vp", GL_VERTEX_SHADER );
	drawShader.LoadShader( "Shaders/Plant.gp", GL_GEOMETRY_SHADER );
	drawShader.LoadShader( "Shaders/Plant.fp", GL_FRAGMENT_SHADER );
	drawShader.BindAttribute( VERTEX_POSITION, "_position" );
	drawShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	drawShader.CompileProgram( );
	drawShader.ObtainUniform( WORLD, "WORLD" );
	drawShader.ObtainUniform( VP, "VP" );

	updateShader.CreateProgram( );
	updateShader.LoadShader( "Shaders/PlantUpdate.cp", GL_COMPUTE_SHADER );
	updateShader.CompileProgram( );
	updateShader.ObtainUniform( TIME, "TIME" );

	Plant::system = this;

	//srand( 16 );
	plantArray.resize( PLANT_AMOUNT );
	dataBufferSize = 0;
	for ( size_t i = 0; i < plantArray.size( ); i++ )
	{
		plantArray[i].location = Vector3::Randomize( { -150, 150 }, { 0, 0 }, { -50, 50 } );

		bool tooClose = false;
		for ( size_t j = 0; j < i; j++ )
		{
			Vector3 distance = plantArray[i].location - plantArray[j].location;
			if ( distance.Length( ) < 10 )
			{
				tooClose = true;
				i--;
				break;
			}
		}
		if ( tooClose )
			continue;

		plantArray[i].bufferIndex = dataBufferSize;
		plantArray[i].Create( );
		dataBufferSize += plantArray[i].bufferLength;
	}




	string message = "Dynamic buffer length: " + std::to_string( dataBufferSize*sizeof( PlantVertex ) ) + " bytes\n";
	GLint number;	glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &number );
	message += "Group Count max: " + std::to_string( number ) + "\n";

	OutputDebugStringA( message.c_str( ) );
	//Plant::staticPositionsBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(Vector4) *numComponents );
	staticDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );
	dynamicDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );


	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );

	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !vertices ) return WinApp::ShowErrorMessage( L"Couldn't read the static data buffer" );

	memcpy( vertices, &vertexData[0], dataBufferSize*sizeof( PlantVertex ) );

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );




	timeVal = 0.0f;

}

void PlantSystem::Update( )
{
#define USE_COMPUTE
#define COMPUTE_FRAME_SKIP 1

	static Clock plantClock;
	timeVal = plantClock.Watch( );

	static int frameSkip = 0;
	if ( frameSkip++ == COMPUTE_FRAME_SKIP )
		frameSkip = 0;

#ifdef USE_COMPUTE

	updateShader.Use( );
	glUniform1f( updateShader.GetUniform( TIME ), timeVal );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );

	glDispatchCompute( dataBufferSize, 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );



#if 0
	for ( size_t i = 0; i < plantArray.size( ); i++ )
	{
		if ( ( i + frameSkip ) % COMPUTE_FRAME_SKIP == 0 )
		{
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );
			plantArray[i].UpdateWithCompute( );
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );
			plantArray[i].ReadDynamicData( );
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );

		}
	}
#endif 
#else

	for ( size_t i = 0; i < plantArray.size( ); i++ )
	{
		if ( ( i + frameSkip ) % COMPUTE_FRAME_SKIP == 0 )
			plantArray[i].Update( timeVal );
	}

#endif
}


void PlantSystem::Render( )
{

	//Set blending
	//glEnable( GL_BLEND );
	glEnable( GL_DEPTH_TEST );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Set antialiasing/multisampling
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	//glEnable( GL_LINE_SMOOTH );
	//glEnable( GL_POLYGON_SMOOTH );
	//glEnable( GL_MULTISAMPLE );

	glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );


	drawShader.Use( );
	glUniformMatrix4fv( drawShader.GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );

	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );
	for ( size_t i = 0; i < plantArray.size( ); i++ )
	{
		plantArray[i].UpdateObject( );
		plantArray[i].Draw( );
	}
}

