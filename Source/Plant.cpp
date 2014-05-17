#include "Plant.h"

#include "GLFactory.h"
#include "GLRenderer.h"

#include "GLSharedTypes.h"
#include <algorithm> //max, min
#include <memory>
#include <ctime>

#include <Windows.h>

#include "WinApp.h"

using Math::Vector3;

#define CHILD_AMOUNT 5
#define LAST_LEVEL 5
#define BASE_LENGTH 10.0f
#define APERTURE 0.18*PI
float			Plant::timeVal;
vector<Plant>	Plant::plantArray;

//GLuint		Plant::staticPositionsBuffer;
GLuint			Plant::dynamicDataBuffer;


Plant::PlantVertex Plant::Transform( const PlantVertex& vertex, uint index )
{

	Vector3 Ro = vertex.position;
	Vector3 No = vertex.normal;

	Vector3 R;
	Vector3 N;

	if ( index == CHILD_AMOUNT - 1)
	{
		R = Ro*RangeRand(0.7,1.0);
		N = No;
	}
	else
	{
		float phi = RangeRand(0.8,1.2)*APERTURE;
		float theta = ( RangeRand(-0.5,0.5) + index)*(2*PI/(CHILD_AMOUNT-1));

		Vector3 S = No.Rotate( theta, Ro.Normalize( ) ).Normalize( );
		R = Ro.Rotate( phi, S ).Normalize( )*Ro.Length( )*RangeRand(0.5f, 0.7f);
		N = No.Rotate( phi, S ).Normalize( );
	}

	float level = vertex.level + 1;
	float delay = UnitRand()*2*PI;

	return { R, level, N, delay };
}

// ----------------
// --- Creation ---
// ----------------

void Plant::InitializeSystem( )
{
	plantArray.resize( 5 );

	for ( UINT i = 0; i < plantArray.size( ); i++ )
	{
		Plant::plantArray[i].location = Vector3::Randomize( { -50, 50 }, { 0, 0 }, { -50, 50 } );
		Plant::plantArray[i].Create( );
	}

	uint numComponents = Plant::plantArray[0].GetComponentCount( );
	//Plant::staticPositionsBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(Vector4) *numComponents );
	dynamicDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) * numComponents );
}



void Plant::Create( )
{
	// --- Initialize tree structure ---

	Component comp;
	comp.parentIndex = 0;
	comp.firstChildIndex = 1;
	comp.childCount = 1;
	components.push_back( comp );

	comp.parentIndex = 0;
	comp.firstChildIndex = 2;
	comp.childCount = CHILD_AMOUNT;
	components.push_back( comp );

	comp = { };
	uint level = 1;
	uint currLevelLeft = 1;
	uint nextLevelCount = CHILD_AMOUNT;

	for ( uint iComp = 1; iComp < components.size( ); iComp++ )
	{
		if ( currLevelLeft == 0 ) // Reached next level in the tree
		{
			if ( ++level == LAST_LEVEL )
				break;
			currLevelLeft = nextLevelCount;
			nextLevelCount = 0;
		}

		components[iComp].firstChildIndex = (int) components.size( );
		components[iComp].childCount = CHILD_AMOUNT;

		nextLevelCount += CHILD_AMOUNT;
		currLevelLeft--;
		comp.parentIndex = iComp;
		comp.firstChildIndex = 0;
		comp.childCount = 0;

		for ( uint iChild = 0; iChild < CHILD_AMOUNT; iChild++ )
			components.push_back( comp );
	}

	// --- Initialize tree data ---

	staticData.resize( components.size( ) );


	staticData[0] = { location, 0, { 1, 0, 0 }, 0 };
	staticData[1] = { { 0, BASE_LENGTH, 0 }, 1, { 1, 0, 0 }, 0 };

	for ( uint i = 2; i < components.size( ); i++ )
	{
		uint parent = components[i].parentIndex;
		uint childnumber = i - components[parent].firstChildIndex;
		staticData[i] = Transform( staticData[parent], childnumber );

	}

	// --- Initialize buffer data ---

	GLShape& shape = object.shape;
	const uint numComp = (uint) components.size( );
	shape.indexCount = numComp * 2;
	//?? shape.vertexCount = numComp * 2;
	shape.primitiveType = GL_LINES;

	ushort* indices = new ushort[numComp * 2];
	memset( indices, 0, sizeof(ushort) *numComp * 2 );

	for ( uint iComp = 1; iComp < numComp; iComp++ )
	{
		indices[iComp * 2] = components[iComp].parentIndex;
		indices[iComp * 2 + 1] = iComp;
	}

	glGenVertexArrays( 1, &shape.vertexArray );
	glBindVertexArray( shape.vertexArray );

	//shape.vertexBuffer = GLFactory::CreateVertexBuffer( VERTEX_POSITION, 4, GL_FLOAT, NULL, sizeof(Vector4) * numComp );

	// Buffer initialization. TODO: transfer to a factory method
	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( GL_ARRAY_BUFFER, handle );
	glBufferData( GL_ARRAY_BUFFER, sizeof(PlantVertex) * numComp, NULL, GL_DYNAMIC_DRAW );

	glEnableVertexAttribArray( VERTEX_POSITION );
	glEnableVertexAttribArray( VERTEX_NORMAL );
	glVertexAttribPointer( VERTEX_POSITION, 4, GL_FLOAT, GL_FALSE, 32, 0 );
	glVertexAttribPointer( VERTEX_NORMAL, 4, GL_FLOAT, GL_FALSE, 32, (void*) 4 );
	shape.vertexBuffer = handle;


	shape.indexBuffer = GLFactory::CreateIndexBuffer( indices, sizeof(ushort) *numComp * 2 );

	delete[] indices;

	timeVal = 1.0f;



	// Initialize GPU buffer with static positions
	staticDataBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(PlantVertex) *GetComponentCount( ) );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );

	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	
	if ( !vertices ) return WinApp::ShowErrorMessage( L"Couldn't read the positions buffer" );


	for ( uint iComp = 0; iComp < components.size( ); iComp++ )
		vertices[iComp] = staticData[iComp];

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	dynamicData.resize( components.size( ) );

}

// --------------
// --- Update ---
// --------------


void Plant::UpdateObject( )
{
	GLShape& shape = object.shape;

	glBindVertexArray( shape.vertexArray );
	glBindBuffer( GL_ARRAY_BUFFER, shape.vertexBuffer );

	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );

	vertices[0] = { { 0, 0, 0 }, 1, { 1, 0, 0 }, 0 };
	for ( uint iComp = 1; iComp < shape.indexCount / 2; iComp++ )
	{
		uint iParent = components[iComp].parentIndex;
		vertices[iComp].position = dynamicData[iComp].position + vertices[iParent].position;
		vertices[iComp].level = dynamicData[iComp].level;
		vertices[iComp].normal = dynamicData[iComp].normal;
		vertices[iComp].delay = dynamicData[iComp].delay;
	}
	glUnmapBuffer( GL_ARRAY_BUFFER );
}



void Plant::Update( )
{
	dynamicData[0] = staticData[0];

	float t = Plant::timeVal;
	Vector3 wind = { -1, 0, 0 };
	wind *= 0.5f;

	for ( uint i = 1; i < components.size( ); i++ )
	{
		const Vector3 Ro = (Vector3) staticData[i].position;
		const Vector3 No = (Vector3) staticData[i].normal;

		Vector3 R;
		Vector3 N;


		//float d = components[i].delay;

		float r = Ro.Length( );

		float Wf = Ro.Cross( wind ).Length();
		Vector3 Wo = Ro +  pow(Wf,0.5)*wind; // equilibrium position with wind
		Wo = Wo.Normalize( )*r;

		float Ao = acos( ( Ro*Wo ) / ( r*r ) ); //angle between Wo and Ro

		Vector3 n = Ro.Cross( Wo ).Normalize( );	//unit vector normal to Rp and Wo
		float w = 10.0f / pow(r,1.0);		//oscilation frequency

		float a = 0.3f*Ao*sin( w*t );

		R = Wo.Rotate( a, n );

		Vector3 pr = Ro.Cross( R ).Normalize( );
		float ar = acos( ( Ro*R ) / ( r*r ) );

		N = No.Rotate( ar, pr );

		float level = staticData[i].level;
		float delay = staticData[i].delay;
		dynamicData[i] = { R, level , N, delay };
	}

}

void Plant::UpdateWithCompute( )
{

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticDataBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );

	uint numComponents = plantArray[0].GetComponentCount( );

	glDispatchCompute( numComponents, 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );


	ReadDynamicData( );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );

}

void Plant::ReadDynamicData( )
{
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicDataBuffer );

	PlantVertex* vertices = (PlantVertex*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !vertices ) return WinApp::ShowErrorMessage( L"Couldn't read the positions buffer" );

	memcpy_s( &dynamicData[0], sizeof(PlantVertex) *dynamicData.size( ), vertices, sizeof(PlantVertex) *dynamicData.size( ) );

	//for ( uint iComp = 0; iComp < components.size( ); iComp++ )
	//	dynamicPositions[iComp] = positions[iComp];

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );
}


void Plant::Draw( )
{
	GLRenderer& renderer = GLRenderer::GetInstance( );
	MatrixStack& stack = renderer.GetMatrixStack( );

	stack.PushMatrix( );
	Matrix44 translation = Matrix44::MakeTranslation( location );
	stack.MultMatrix( translation );
	object.Draw( );
	stack.PopMatrix( );
}

