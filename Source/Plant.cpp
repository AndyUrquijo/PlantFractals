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

float			Plant::timeVal;
vector<Plant>	Plant::plantArray;

//GLuint		Plant::staticPositionsBuffer;
GLuint			Plant::dynamicPositionsBuffer;


Vector3 Transorm( const Vector3& Ro, uint index )
{
#define PHI 0.03f*PI
#define THETA 0.66f*PI

	Vector3 R;
	if ( index == 3 )
	{
		R = Ro*0.9f;
	}
	else
	{
		float phi = PHI;
		float theta = index*THETA;

		R = Vector3::RandomRotation( Ro, theta, theta, phi, phi );
		R = R.Normalize( )*Ro.Length( )*0.6f;
	}
	return R;
}

// ----------------
// --- Creation ---
// ----------------

void Plant::Create( )
{
#define CHILD_AMOUNT 4
#define LAST_LEVEL 7
#define BASE_LENGTH 10.0f


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

	staticPositions.resize( components.size( ) );


	staticPositions[0] = Vector4( location, 1 );
	staticPositions[1] = { 0, BASE_LENGTH, 0, 1 };
	for ( uint i = 2; i < components.size( ); i++ )
	{
		uint parent = components[i].parentIndex;
		uint childnumber = i - components[parent].firstChildIndex;
		staticPositions[i] = Vector4( Transorm( Vector3( staticPositions[parent] ), childnumber ), 0 );
		components[i].delay = RangeRand( 0, 2 * Math::PI );
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

	shape.vertexBuffer = GLFactory::CreateVertexBuffer( VERTEX_POSITION, 4, GL_FLOAT, NULL, sizeof(Vector4) * numComp );
	shape.indexBuffer = GLFactory::CreateIndexBuffer( indices, sizeof(ushort) *numComp * 2 );

	delete[] indices;

	timeVal = 1.0f;



	// Initialize GPU buffer with static positions
	staticPositionsBuffer = GLFactory::CreateShaderStorageBuffer( NULL, sizeof(Vector4) *GetComponentCount( ) );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticPositionsBuffer );

	Vector4* positions = (Vector4*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !positions ) return WinApp::ShowErrorMessage( L"Couldn't read the positions buffer" );


	for ( uint iComp = 0; iComp < components.size( ); iComp++ )
		positions[iComp] = staticPositions[iComp];

	glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );

	dynamicPositions.resize( components.size( ) );

}


// --------------
// --- Update ---
// --------------


void Plant::UpdateObject( )
{
	GLShape& shape = object.shape;

	glBindVertexArray( shape.vertexArray );
	glBindBuffer( GL_ARRAY_BUFFER, shape.vertexBuffer );

	Vector4* positions = (Vector4*) glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	positions[0] = { 0, 0, 0, 1 };
	for ( uint iComp = 1; iComp < shape.indexCount / 2; iComp++ )
	{
		uint iParent = components[iComp].parentIndex;;
		positions[iComp] = dynamicPositions[iComp] + positions[iParent];
	}
	glUnmapBuffer( GL_ARRAY_BUFFER );
}



void Plant::Update( )
{
	dynamicPositions[0] = staticPositions[0];

	float t = Plant::timeVal;
	Vector3 wind = { -1, 0, 0 };
	wind *= 0.5f;

	for ( uint i = 1; i < components.size( ); i++ )
	{
		Vector3 R = (Vector3) dynamicPositions[i];
		Vector3 Ro = (Vector3) staticPositions[i];

		//float d = components[i].delay;

		float r = Ro.Length( );

		float Wf = Ro.Cross( wind ).Length( );
		Vector3 Wo = Ro + sqrt( Wf ) *wind; // equilibrium position with wind
		Wo = Wo.Normalize( )*r;

		float Ao = acos( ( Ro*Wo ) / ( r*r ) ); //angle between Wo and Ro

		Vector3 n = Ro.Cross( Wo ).Normalize( );	//unit vector normal to Rp and Wo
		float w = 10.0f / r;		//oscilation frequency

		R = Wo.Rotate( 0.3f*Ao*sin( w*t ), n );

		dynamicPositions[i] = Vector4( R, 0 );
	}

}

void Plant::UpdateWithCompute( )
{

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, staticPositionsBuffer );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicPositionsBuffer );

	uint numComponents = plantArray[0].GetComponentCount( );

	glDispatchCompute( numComponents, 1, 1 );
	glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );


	ReadDynamicData( );

	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, 0 );
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, 0 );

}

void Plant::ReadDynamicData( )
{
	glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 1, dynamicPositionsBuffer );

	Vector4* positions = (Vector4*) glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE );
	if ( !positions ) return WinApp::ShowErrorMessage( L"Couldn't read the positions buffer" );

	memcpy_s( &dynamicPositions[0], sizeof(Vector4) *dynamicPositions.size( ), positions, sizeof(Vector4) *dynamicPositions.size( ) );

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

