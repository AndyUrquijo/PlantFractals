#include "PlantSystem.h"
#include "Clock.h"
#include "GLRenderer.h"
#include "GLFactory.h"
#include <Windows.h>
#include <string>
using std::string;
#include "WinApp.h"

#define PLANT_AMOUNT 5
#define COPY_AMOUNT 5
#define FOREST_AREA 10

#define PLANT_SEPARATION 15
#define CLONE_SEPARATION PLANT_SEPARATION*1

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
	drawShader.ObtainUniform( DISPLACEMENT, "DISPLACEMENT" );

	drawLeavesShader.CreateProgram( );
	drawLeavesShader.LoadShader( "Shaders/Plant.vp", GL_VERTEX_SHADER );
	drawLeavesShader.LoadShader( "Shaders/Leaf.gp", GL_GEOMETRY_SHADER );
	drawLeavesShader.LoadShader( "Shaders/Leaf.fp", GL_FRAGMENT_SHADER );
	drawLeavesShader.BindAttribute( VERTEX_POSITION, "_position" );
	drawLeavesShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	drawLeavesShader.CompileProgram( );
	drawLeavesShader.ObtainUniform( VP, "VP" );
	drawLeavesShader.ObtainUniform( DISPLACEMENT, "DISPLACEMENT" );

	updateShader.CreateProgram( );
	updateShader.LoadShader( "Shaders/PlantUpdate.cp", GL_COMPUTE_SHADER );
	updateShader.CompileProgram( );
	updateShader.ObtainUniform( TIME, "TIME" );

	Plant::system = this;

	srand( 100 );
	plantArray.resize( PLANT_AMOUNT );
	dataBufferSize = 0;
	for ( int iPl = 0; iPl < plantArray.size( ); iPl++ )
		plantArray[iPl].locations.resize( COPY_AMOUNT );

	for ( int iPl = 0; iPl < plantArray.size( ); iPl++ )
	{

		for ( size_t iLc = 0; iLc < plantArray[iPl].locations.size( ); iLc++ )
		{
			plantArray[iPl].locations[iLc] = Vector3::Randomize( { -10, 10 }, { 0, 0 }, { -10, 10 } ) * FOREST_AREA;

			bool tooClose = false;
			for ( int jPl = 0; jPl <= iPl; jPl++ )
			{
				size_t boundLc = plantArray[jPl].locations.size( );
				float minDistance = PLANT_SEPARATION;
				if ( jPl == iPl )
				{
					boundLc = iLc;
					minDistance = CLONE_SEPARATION;
				}

				for ( size_t jLc = 0; jLc < boundLc; jLc++ )
				{
					Vector3 distance = plantArray[iPl].locations[iLc] - plantArray[jPl].locations[jLc];
					if ( distance.Length( ) < minDistance )
					{
						tooClose = true;
						iLc--;
						break;
					}
				}
				if ( tooClose )
					break;
			}
			if ( tooClose )
				continue;
		}

		plantArray[iPl].bufferIndex = dataBufferSize;
		plantArray[iPl].Create( );
		dataBufferSize += plantArray[iPl].bufferLength;
	}


	// Initialize GPU vertex data

	staticDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );
	dynamicDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );

	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !vertices ) return WinApp::ShowErrorMessage( L"Couldn't read the static data buffer" );

	memcpy( vertices, &vertexData[0], dataBufferSize*sizeof( PlantVertex ) );

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	// Initialize GPU parent index data

	parentIndexBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(UINT) * dataBufferSize );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, parentIndexBuffer );

	PlantVertex* indices = (PlantVertex*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !indices ) return WinApp::ShowErrorMessage( L"Couldn't read the parent index data buffer" );

	memcpy( indices, &parentIndexData[0], dataBufferSize*sizeof( UINT ) );

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
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, parentIndexBuffer );

	glDispatchCompute( 100, dataBufferSize / 100 + 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );


	
	// Translate positions along the hiearchy
	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );
	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].UpdateObject( vertices );

	glUnmapBuffer( GL_ARRAY_BUFFER );
	glBindBuffer( GL_ARRAY_BUFFER, NULL );
	
}


void PlantSystem::Render( )
{

	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );

	glEnableVertexAttribArray( VERTEX_POSITION );
	glEnableVertexAttribArray( VERTEX_NORMAL );
	glVertexAttribPointer( VERTEX_POSITION, 4, GL_FLOAT, GL_FALSE, 32, 0 );
	glVertexAttribPointer( VERTEX_NORMAL, 4, GL_FLOAT, GL_FALSE, 32, (void*) 16 );



	drawShader.Use( );
	glLineWidth( 4.0f );
	glUniformMatrix4fv( drawShader.GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_BRANCHES );

	

	drawLeavesShader.Use( );
	glLineWidth( 8.0f );
	glUniformMatrix4fv( drawShader.GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_LEAVES );

	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

}