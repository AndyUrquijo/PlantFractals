#include "GLGeometry.h"
#include <iostream>

#include "GLFactory.h"
#include <GL\glew.h>

using namespace Math;

GLShape GLGeometry::MakeCube( CUBE_DESC& description )
{
#define CUBE_VERTEX_NUM 8
#define CUBE_INDEX_NUM 36
	GLShape cube;

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
