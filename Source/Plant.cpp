#include "Plant.h"

#include "GLFactory.h"
#include "GLRenderer.h"

#include "GLSharedTypes.h"
#include <algorithm> //max, min
#include <memory>

using Math::Vector3;

Plant::Plant( )
{
}


Plant::~Plant( )
{
}

void BranchingFunction( int level, int* min, int* max )
{
#define PHASE 5
	if ( level < PHASE )
	{
		*min = level / 2 + 1;
		*max = ( level*level ) / 4 + 1;
	}
	else
	{
		*min = std::max( 0, PHASE / 2 + ( PHASE - level ) / 2 );
		*max = std::max( 0, PHASE * PHASE / 4 - ( PHASE - level )*( PHASE - level ) / 4 );
	}
}



void Plant::Grow( )
{
#define BASE_LENGTH 10.0f

	//location = { 0, 0, 0 };

	Component child;
	child.parentIndex = 0;
	components.push_back( child );
	int level, min, max;
	int currLevelNum;
	int nextLevelNum;
	int numChildren;
	float angle;
	Math::Vector3 axis;

	//Root element
	level = 0;

	child.displacement = { 0, 0, 0 };
	child.parentIndex = 0;
	components.push_back( child );
	child.displacement = { 0, BASE_LENGTH, 0 };
	components.push_back( child );

	currLevelNum = 1;
	nextLevelNum = 1;

	for ( uint iComp = 2; iComp < components.size( ); iComp++ )
	{
		if ( --currLevelNum == 0 ) // Reached next level in the tree
		{
			if ( ++level == 8 )
				break;
			currLevelNum = nextLevelNum;
			nextLevelNum = 0;
		}

		numChildren = 3;
		nextLevelNum += numChildren;
		float theta = 0;
		float phi = 0.03*PI;
		for ( uint iChild = 0; iChild < numChildren; iChild++ )
		{
			theta += 2 * PI / float( numChildren );
			child.displacement = Vector3::RandomRotation( components[iComp].displacement,
														  theta, theta, phi, phi );
			child.displacement = child.displacement.Normalize( );
			child.parentIndex = iComp;
			child.displacement *= components[child.parentIndex].displacement.Length()*0.7f;
			//;*pow( 0.7f, float( level ) );
			components.push_back( child );
		}
	}
}

void Plant::InitializeObject( )
{
	GLShape& shape = object.shape;
	const uint numComp = components.size( );
	shape.size = numComp * 2;
	shape.indexed = true;
	shape.primitiveType = GL_LINES;

	Vector3* positions = new Vector3[numComp];
	memset( positions, 0, sizeof(Vector3) *numComp );
	ushort* indices = new ushort[numComp * 2];

	uint iCurr;

	for ( uint iComp = 1; iComp < numComp; iComp++ )
	{
		iCurr = iComp;
		while ( iCurr != 0 )
		{
			positions[iComp] += components[iCurr].displacement;
			iCurr = components[iCurr].parentIndex;
		}
		positions[iComp] += components[0].displacement;

		indices[iComp * 2] = components[iComp].parentIndex;
		indices[iComp * 2 + 1] = iComp;
	}


	glGenVertexArrays( 1, &shape.vertexArray );
	glBindVertexArray( shape.vertexArray );

	GLFactory::CreateVertexBuffer( VERTEX_POSITION, 3, GL_FLOAT, positions, sizeof(Vector3) * numComp );
	GLFactory::CreateIndexBuffer( indices, sizeof(ushort) *numComp * 2 );

	delete[] positions;
	delete[] indices;
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

