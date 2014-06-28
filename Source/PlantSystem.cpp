#include "PlantSystem.h"
#include "Clock.h"
#include "GLRenderer.h"
#include "GLFactory.h"
#include <Windows.h>
#include <string>
using std::string;
#include "WinApp.h"

#define PLANT_AMOUNT 10

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
	drawShader.ObtainUniform( VP, "VP" );

	drawLeavesShader.CreateProgram( );
	drawLeavesShader.LoadShader( "Shaders/Plant.vp", GL_VERTEX_SHADER );
	drawLeavesShader.LoadShader( "Shaders/Leaf.gp", GL_GEOMETRY_SHADER );
	drawLeavesShader.LoadShader( "Shaders/Leaf.fp", GL_FRAGMENT_SHADER );
	drawLeavesShader.BindAttribute( VERTEX_POSITION, "_position" );
	drawLeavesShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	drawLeavesShader.CompileProgram( );
	drawLeavesShader.ObtainUniform( VP, "VP" );

	updateShader.CreateProgram( );
	updateShader.LoadShader( "Shaders/PlantUpdate.cp", GL_COMPUTE_SHADER );
	updateShader.CompileProgram( );
	updateShader.ObtainUniform( TIME, "TIME" );

	Plant::system = this;

	srand( 100 );
	plantArray.resize( PLANT_AMOUNT );
	dataBufferSize = 0;
	for ( int i = 0; i < plantArray.size( ); i++ )
	{
		plantArray[i].location = Vector3::Randomize( { -10,10 }, { 0, 0 }, { -10, 10 } )*20;
		
		bool tooClose = false;
		for ( size_t j = 0; j < i; j++ )
		{
			Vector3 distance = plantArray[i].location - plantArray[j].location;
			if ( distance.Length( ) < 20 )
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
	static Clock plantClock;
	timeVal = plantClock.Watch( );

	updateShader.Use( );
	glUniform1f( updateShader.GetUniform( TIME ), timeVal );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );

	glDispatchCompute( 100, dataBufferSize/100 + 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );


	// Translate positions along the hiearchy
	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );
	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].UpdateObject( vertices );

	glUnmapBuffer( GL_ARRAY_BUFFER );
}


void PlantSystem::Render( )
{

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



	drawShader.Use( );
	glLineWidth(4.0f);
	glUniformMatrix4fv( drawShader.GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );
	
	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_BRANCHES );

	
	drawLeavesShader.Use( );
	glLineWidth(8.0f);
	glUniformMatrix4fv( drawShader.GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );
	
	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_LEAVES );
		
  
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

}