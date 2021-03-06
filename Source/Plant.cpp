/*
.	Author:		Andres Urquijo
.	Filename:	Plant.cpp	
*/
#include "Plant.h"

#include "GLFactory.h"
#include "GLRenderer.h"

#include "GLSharedTypes.h"
#include <algorithm> //max, min
#include <memory>
#include <ctime>

#include <Windows.h>

#include "Clock.h"

#include "WinApp.h"

using Math::Vector3;


#define LEAF_AMOUNT		2
#define LAST_LEVEL		7
#define LEAF_LEVEL		LAST_LEVEL - 2
#define BASE_LENGTH		10.0f
#define APERTURE		0.18f*PI
#define LEAF_LENGTH		0.2f	
//#define LEAF_WIDTH		0.4f	


/// Random ranges
#define CHILD_AMOUNT		2,		4
#define STRAIGHT_RATIO		0.8f,	1.0f
#define TWIST_RATIO			0.4f,	0.7f
#define DISTRIB_RATIO		0.0f,	1.0f
#define APERTURE_RATIO		0.8f,	1.2f

PlantSystem* Plant::system;



PlantVertex Plant::Transform( const PlantVertex& vertex, uint index, uint count, TransformType type )
{

	Vector3 Ro = vertex.position;
	Vector3 No = vertex.normal;

	Vector3 R;
	Vector3 N;

	float level;
	float delay;

	switch ( type )
	{
		case BRANCH_STRAIGHT:
		{ {
			R = Ro*RangeRand( STRAIGHT_RATIO );
			N = No.Normalize( );;
			level = vertex.level + 1;
		} } break;
		break;
		case BRANCH_FORK:
		{ {
			float phi = RangeRand( APERTURE_RATIO )*APERTURE;
			float theta = ( RangeRand( DISTRIB_RATIO ) + index )*( 2 * PI / ( count - 1 ) );

			Vector3 S = No.Rotate( theta, Ro.Normalize( ) ).Normalize( );
			R = Ro.Rotate( phi, S ).Normalize( )*Ro.Length( )*RangeRand( TWIST_RATIO );
			N = No.Rotate( phi, S ).Normalize( );

			level = vertex.level + 1;
		} } break;
		case LEAF:
		{ {
			float phi = RangeRand( APERTURE_RATIO )*APERTURE;
			float theta = RangeRand( 0.0, 2 * PI );

			Vector3 S = No.Rotate( theta, Ro.Normalize( ) ).Normalize( );

			R = Ro.Rotate( phi, S ).Normalize( )*LEAF_LENGTH;
			N = Vector3::RandomOrthonormal(R);
			//N = R.Cross(Vector3::RIGHT).Normalize();
			//R = Vector3(0.707f,0.707f,0)*LEAF_LENGTH;
			//N = Vector3(-0.707f,0.707f,0);
			
			level = RangeRand( 0, 1 )*(-1);

		} } break;
	}


	delay = UnitRand( ) * 2 * PI;

	return { R, level, N, delay };
}



// ----------------
// --- Creation ---
// ----------------


void Plant::Create( )
{
	// --- Initialize tree structure ---

	components.resize( 0 );

	Component comp;
	comp.parentIndex = 0;
	comp.firstChildIndex = 1;
	comp.childCount = 1;
	components.push_back( comp );

	comp.parentIndex = 0;
	comp.firstChildIndex = 2;
	comp.childCount = (GLushort)floorf( RangeRand( CHILD_AMOUNT ) + 0.5f );
	components.push_back( comp );

	comp = { };
	uint level = 1;
	uint currLevelLeft = 1;
	uint nextLevelCount = comp.childCount;

	uint firstLeafBranchIndex = 0;

	//Create Branches

	for ( uint iComp = 1;; iComp++ )
	{
		if ( currLevelLeft == 0 ) // Reached next level in the tree
		{
			++level;
			if ( level == LAST_LEVEL )
				break;
			else if ( !firstLeafBranchIndex && level == LEAF_LEVEL )
				firstLeafBranchIndex = iComp;

			currLevelLeft = nextLevelCount;
			nextLevelCount = 0;
		}

		components[iComp].firstChildIndex = (int) components.size( );
		components[iComp].childCount = (GLushort)floorf( RangeRand( CHILD_AMOUNT ) + 0.5f );


		nextLevelCount += components[iComp].childCount;
		currLevelLeft--;
		comp.parentIndex = iComp;
		comp.firstChildIndex = 0;
		comp.childCount = 0;

		for ( uint iChild = 0; iChild < components[iComp].childCount; iChild++ )
			components.push_back( comp );
	}
	//end of branch creation

	leavesIndex = (uint)components.size( );

	// Add leaves as children of the modes on latest level
	for ( uint iComp = firstLeafBranchIndex; iComp < leavesIndex; iComp++ )
	{
		//components[iComp].firstChildIndex = (int) components.size( );
		//components[iComp].childCount = LEAF_AMOUNT;

		comp.parentIndex = iComp;
		comp.firstChildIndex = 0;
		comp.childCount = 0;

		for ( uint iChild = 0; iChild < LEAF_AMOUNT; iChild++ )
			components.push_back( comp );
	}

	bufferLength = (uint) components.size( );

	// --- Initialize tree data ---

	system->vertexData.resize( bufferIndex + components.size( ) );
	system->parentIndexData.resize( bufferIndex + components.size( ) );


	system->vertexData[bufferIndex + 0] = { { 0, 0, 0 }, 1, { 1, 0, 0 }, 0 };
	system->vertexData[bufferIndex + 1] = { { 0, BASE_LENGTH, 0 }, 2, { 1, 0, 0 }, 0 };

	for ( uint iV = 2; iV < components.size( ); iV++ )
	{
		uint parent = components[iV].parentIndex;
		uint childnumber = iV - components[parent].firstChildIndex;
		uint siblingCount = components[parent].childCount;
		TransformType type;
		if ( iV >= leavesIndex )
			type = LEAF;
		else if ( childnumber == siblingCount - 1 )
			type = BRANCH_STRAIGHT;
		else
			type = BRANCH_FORK;

		system->vertexData[bufferIndex + iV] = Transform( system->vertexData[bufferIndex + parent], childnumber, siblingCount, type );
		system->parentIndexData[bufferIndex + iV] = parent;
	}

	// --- Initialize index buffer data ---

	// branches data
	{
		GLShapeIndexed& shape = branchesObject.shape;
		shape.indexCount = leavesIndex * 2;
		shape.primitiveType = GL_LINES;

		uint* bIndices = new uint[shape.indexCount];
		memset( bIndices, 0, sizeof(uint) *shape.indexCount );

		for ( uint iComp = 1; iComp < leavesIndex; iComp++ )
		{
			bIndices[iComp * 2] = bufferIndex + components[iComp].parentIndex;
			bIndices[iComp * 2 + 1] = bufferIndex + iComp;
		}

		shape.indexBuffer = GLFactory::CreateIndexBuffer( bIndices, sizeof(uint) * shape.indexCount );

		delete[] bIndices;
	}

	// leaves data
	{
		GLShapeIndexed& shape = leavesObject.shape;
		shape.indexCount = ( bufferLength - leavesIndex ) * 3;
		shape.primitiveType = GL_TRIANGLES;

		uint* lIndices = new uint[shape.indexCount];
		memset( lIndices, 0, sizeof(uint) *shape.indexCount );

		for ( uint iComp = leavesIndex; iComp < bufferLength; iComp++ )
		{
			uint iIndex = iComp - leavesIndex;
			GLushort& iParent = components[iComp].parentIndex;
			GLushort& iGrandparent = components[iParent].parentIndex;
			lIndices[iIndex * 3 + 0] = bufferIndex + iGrandparent;
			lIndices[iIndex * 3 + 1] = bufferIndex + iParent;
			lIndices[iIndex * 3 + 2] = bufferIndex + iComp;
		}

		shape.indexBuffer = GLFactory::CreateIndexBuffer( lIndices, sizeof(uint) * shape.indexCount );

		delete[] lIndices;
	}
}

// --------------
// --- Update ---
// --------------


void Plant::UpdateObject( PlantVertex* const vertices )
{

	//PlantVertex* vertices = (PlantVertex*) glMapBufferRange( GL_ARRAY_BUFFER, bufferIndex*sizeof(PlantVertex), bufferLength*sizeof(PlantVertex), GL_READ_WRITE );

	vertices[bufferIndex + 0].position = { 0, 0, 0 };
	vertices[bufferIndex + 0].normal = { 1, 0, 0 };
	vertices[bufferIndex + 0].level = 1;
	vertices[bufferIndex + 0].delay = 0;

	for ( uint iComp = 1; iComp < leavesIndex; iComp++ )
	{
		uint iParent = components[iComp].parentIndex;
		vertices[bufferIndex + iComp].position += vertices[bufferIndex + iParent].position;
	}

	
}


void Plant::Draw( DrawType drawtype )
{

	
	for ( size_t i = 0; i < locations.size( ); i++ )
	{

		glUniform3f( GLRenderer::GetShader( ).GetUniform( DISPLACEMENT ), 
					 locations[i].x,
					 locations[i].y,
					 locations[i].z);

		switch ( drawtype )
		{
						 case Plant::DRAW_BRANCHES:
						 glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, branchesObject.shape.indexBuffer );
						 branchesObject.shape.Draw( );
						 break;
						 case Plant::DRAW_LEAVES:
						 glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, leavesObject.shape.indexBuffer );
						 leavesObject.shape.Draw( );
						 break;
		}
	}

}
