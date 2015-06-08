#include "GLGeometry.h"
#include <iostream>

#include "GLFactory.h"
#include <GL\glew.h>

using namespace Math;

GLShapeIndexed GLGeometry::MakeCube( CUBE_DESC& description )
{
#define CUBE_VERTEX_NUM 8
#define CUBE_INDEX_NUM 36
	GLShapeIndexed cube;

	cube.indexCount = CUBE_INDEX_NUM;
	cube.primitiveType = GL_TRIANGLES;

	Vector3 positions[CUBE_VERTEX_NUM] =
	{
		{ -1, -1, -1 },
		{ +1, -1, -1 },
		{ -1, +1, -1 },
		{ +1, +1, -1 },
		{ -1, -1, +1 },
		{ +1, -1, +1 },
		{ -1, +1, +1 },
		{ +1, +1, +1 },
	};
	Vector4 colors[CUBE_VERTEX_NUM] =
	{
		{ 1, 0, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 1, 1, 0, 1 },
		{ 0, 1, 1, 1 },
		{ 1, 0, 1, 1 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1, 1 },
	};

	GLushort indices[CUBE_INDEX_NUM] =
	{
		0, 5, 4, 0, 1, 5,
		2, 7, 3, 2, 6, 7,
		0, 6, 2, 0, 4, 6,
		1, 7, 5, 1, 3, 7,
		0, 3, 1, 0, 2, 3,
		4, 7, 5, 4, 6, 7,
	};

	glGenVertexArrays( 1, &cube.vertexArray );
	glBindVertexArray( cube.vertexArray );

	GLFactory::CreateVertexBuffer( VERTEX_POSITION, 3, GL_FLOAT, positions, sizeof positions );
	GLFactory::CreateVertexBuffer( VERTEX_COLOR, 4, GL_FLOAT, colors, sizeof colors );
	GLFactory::CreateIndexBuffer( indices, sizeof indices );


	return cube;
}


GLShape GLGeometry::MakeQuad( QUAD_DESC& description )
{
#define QUAD_VERTEX_NUM 4
	GLShape quad;

	quad.vertexCount = QUAD_VERTEX_NUM;
	quad.primitiveType = GL_TRIANGLE_STRIP;
	Vector2& dim = description.dimensions;

	Vector3 positions[QUAD_VERTEX_NUM] =
	{
		{ -dim.x, 0, -dim.y },
		{ -dim.x, 0, +dim.y },
		{ +dim.x, 0, -dim.y },
		{ +dim.x, 0, +dim.y },
	};

	glGenVertexArrays( 1, &quad.vertexArray );
	glBindVertexArray( quad.vertexArray );
	quad.vertexBuffer = GLFactory::CreateVertexBuffer( VERTEX_POSITION, 3, GL_FLOAT, positions, sizeof positions );


	return quad;
}

GLColorShape GLGeometry::MakePlaneGrid( PLANE_GRID_DESC description )
{
	uint numCols = (uint)description.layers.x + 1;
	uint numRows = (uint)description.layers.y + 1;
	Vector2 cellSize = { description.size.x / description.layers.x, description.size.y / description.layers.y };

	Vector3 startPoint = description.center - Vector3(description.size.x,description.size.y,0)*0.5f;
	uint vertexAmount = numCols*numRows;
	uint indexAmount = ( numCols - 1 )*( numRows - 1 )*6;
	GLColorShape planeGrid;

	planeGrid.indexCount = indexAmount;
	planeGrid.vertexCount = vertexAmount;
	planeGrid.primitiveType = GL_TRIANGLES;

	Vector3* positions = new Vector3[vertexAmount];
	Vector4* colors = new Vector4[vertexAmount];

	for ( size_t i = 0; i < numCols; i++ )
	for ( size_t j = 0; j < numRows; j++ )
	{
		positions[i + j*numCols] = Vector3( i*cellSize.x, j*cellSize.y, 0 ) + startPoint;
		colors[i + j*numCols] = { 0, 0, 0, 0+6 };
	}

	GLuint* indices = new GLuint[indexAmount];

	for ( size_t i = 0; i < numCols - 1; i++ )
	for ( size_t j = 0; j < numRows - 1; j++ )
	{
		indices[( i + j*(numCols-1) ) * 6 + 0] = (i+0) + (j+0)*numCols;
		indices[( i + j*(numCols-1) ) * 6 + 1] = (i+1) + (j+0)*numCols;
		indices[( i + j*(numCols-1) ) * 6 + 2] = (i+1) + (j+1)*numCols;
		indices[( i + j*(numCols-1) ) * 6 + 3] = (i+0) + (j+0)*numCols;
		indices[( i + j*(numCols-1) ) * 6 + 4] = (i+0) + (j+1)*numCols;
		indices[( i + j*(numCols-1) ) * 6 + 5] = (i+1) + (j+1)*numCols;
	}

	glGenVertexArrays( 1, &planeGrid.vertexArray );
	glBindVertexArray( planeGrid.vertexArray );

	planeGrid.vertexBuffer = GLFactory::CreateVertexBuffer( VERTEX_POSITION, 3, GL_FLOAT, positions, sizeof(Vector3)*vertexAmount );
	planeGrid.colorBuffer = GLFactory::CreateVertexBuffer( VERTEX_COLOR, 4, GL_FLOAT, colors, sizeof(Vector4)*vertexAmount );
	planeGrid.indexBuffer = GLFactory::CreateIndexBuffer( indices, sizeof(GLuint)*indexAmount );

	delete[] positions;
	delete[] colors;
	delete[] indices;

	return planeGrid;
}
