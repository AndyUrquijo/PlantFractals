/*
.	Author:		Andres Urquijo
.	Filename:	PlantSystem.cpp
*/
#include "PlantSystem.h"
#include "Clock.h"
#include "GLRenderer.h"
#include "GLFactory.h"
#include <Windows.h>
#include <string>
using std::string;
#include "WinApp.h"

#define PLANT_AMOUNT 12
#define COPY_AMOUNT 20
#define FOREST_AREA 25

#define PLANT_SEPARATION 15
#define CLONE_SEPARATION PLANT_SEPARATION*1

void PlantSystem::Initialize( )
{

	Plant::system = this;
	timeVal = 0.0f;

	InitializeShaders();
	InitializePlants();
	InitializeBuffers();

}

void PlantSystem::InitializeShaders()
{
	drawShader.CreateProgram( );
	drawShader.LoadShader( "Plant.vp", GL_VERTEX_SHADER );
	drawShader.LoadShader( "Plant.gp", GL_GEOMETRY_SHADER );
	drawShader.LoadShader( "Plant.fp", GL_FRAGMENT_SHADER );
	drawShader.BindAttribute( VERTEX_POSITION, "_position" );
	drawShader.BindAttribute( VERTEX_LEVEL, "_level" );
	drawShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	drawShader.BindAttribute( VERTEX_DELAY, "_delay" );
	drawShader.CompileProgram( );
	drawShader.ObtainUniform( VP, "VP" );
	drawShader.ObtainUniform( DISPLACEMENT, "DISPLACEMENT" );

	drawLeavesShader.CreateProgram( );
	drawLeavesShader.LoadShader( "Plant.vp", GL_VERTEX_SHADER );
	drawLeavesShader.LoadShader( "Leaf.gp", GL_GEOMETRY_SHADER );
	drawLeavesShader.LoadShader( "Leaf.fp", GL_FRAGMENT_SHADER );
	drawLeavesShader.BindAttribute( VERTEX_POSITION, "_position" );
	drawLeavesShader.BindAttribute( VERTEX_LEVEL, "_level" );
	drawLeavesShader.BindAttribute( VERTEX_NORMAL, "_normal" );
	drawLeavesShader.BindAttribute( VERTEX_DELAY, "_delay" );	
	drawLeavesShader.CompileProgram( );
	drawLeavesShader.ObtainUniform( VP, "VP" );
	drawLeavesShader.ObtainUniform( DISPLACEMENT, "DISPLACEMENT" );
	drawLeavesShader.ObtainUniform( CAMERA_POS, "CAMERA_POS" );

	updateShader.CreateProgram( );
	updateShader.LoadShader( "PlantUpdate.cp", GL_COMPUTE_SHADER );
	updateShader.CompileProgram( );
	updateShader.ObtainUniform( TIME, "TIME" );
}

void PlantSystem::InitializePlants()
{
	srand( 100 );
	plantArray.resize( PLANT_AMOUNT );
	dataBufferSize = 0;
	for ( size_t iPl = 0; iPl < plantArray.size( ); iPl++ )
		plantArray[iPl].locations.resize( COPY_AMOUNT );

	for ( size_t iPl = 0; iPl < plantArray.size( ); iPl++ )
	{

		for ( size_t iLc = 0; iLc < plantArray[iPl].locations.size( ); iLc++ )
		{
			plantArray[iPl].locations[iLc] = Vector3::Randomize( { -10, 10 }, { 0, 0 }, { -10, 10 } ) * FOREST_AREA;

			bool tooClose = false;
			for ( size_t jPl = 0; jPl <= iPl; jPl++ )
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

}

void PlantSystem::InitializeBuffers()
{
	staticDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );
	UpdateGLBuffer((PlantVertex*)&vertexData[0], dataBufferSize, GL_SHADER_STORAGE_BUFFER);
	
	dynamicDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );
	ZeroGLBuffer( sizeof(PlantVertex) * dataBufferSize, GL_SHADER_STORAGE_BUFFER);
	
	translatedDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * dataBufferSize );
	ZeroGLBuffer( sizeof(PlantVertex) * dataBufferSize, GL_SHADER_STORAGE_BUFFER);

	parentIndexBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(UINT) * dataBufferSize );
	UpdateGLBuffer(&parentIndexData[0], dataBufferSize, GL_SHADER_STORAGE_BUFFER);
}


void PlantSystem::Update( )
{
	static Clock plantClock;
	timeVal = plantClock.Watch( );

	updateShader.Use( );
	glUniform1f( updateShader.GetUniform( TIME ), timeVal );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, translatedDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, parentIndexBuffer );

	glDispatchCompute( 100, dataBufferSize / 100 + 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 2, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, 0 );


#if 1 // CPU translations
	// Translate positions along the hiearchy
	
	
	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );
	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].UpdateObject( vertices );
	

	glUnmapBuffer( GL_ARRAY_BUFFER );
	glBindBuffer( GL_ARRAY_BUFFER, NULL );
#endif
}



void SetAttribute( GLVertexAttribute attribute, GLuint size, GLuint stride, GLuint offset )
{
	glEnableVertexAttribArray( attribute );
	glVertexAttribPointer( attribute, size, GL_FLOAT, GL_FALSE, stride*4, (void*)(offset*4) );
}

void PlantSystem::Render( )
{
	/*
	glBindBuffer( GL_ARRAY_BUFFER, parentIndexBuffer );
	UINT* indices = (UINT*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	glUnmapBuffer( GL_ARRAY_BUFFER );


	glBindBuffer( GL_ARRAY_BUFFER, translatedDataBuffer );
	PlantVertex* translated = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	glUnmapBuffer( GL_ARRAY_BUFFER );
	*/
	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );
	PlantVertex* dynamic = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	glUnmapBuffer( GL_ARRAY_BUFFER );
	

	drawShader.Use( );

	glBindBuffer( GL_ARRAY_BUFFER, dynamicDataBuffer );

	SetAttribute(VERTEX_POSITION,	3, 8, 0);
	SetAttribute(VERTEX_LEVEL,		1, 8, 3);
	SetAttribute(VERTEX_NORMAL,		3, 8, 4);
	SetAttribute(VERTEX_DELAY,		1, 8, 7);




	glLineWidth( 0.4f );
	glUniformMatrix4fv( GLRenderer::GetShader( ).GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_BRANCHES );


	drawLeavesShader.Use( );

	Vector3 camPos = GLRenderer::GetCameraPos();
	glUniform3f( GLRenderer::GetShader( ).GetUniform( CAMERA_POS ), 
				 camPos.x,
				 camPos.y,
				 camPos.z);

	glLineWidth( 4.0f );
	glUniformMatrix4fv( GLRenderer::GetShader( ).GetUniform( VP ), 1, GL_FALSE, GLRenderer::GetViewProjectionMatrix( ).elm );

	for ( size_t i = 0; i < plantArray.size( ); i++ )
		plantArray[i].Draw( Plant::DRAW_LEAVES );
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

}