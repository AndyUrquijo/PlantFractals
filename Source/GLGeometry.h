#pragma once

#include "Math.h"
#include "GLShape.h"

struct GLGeometry
{
	struct CUBE_DESC{ Math::Vector3 dimensions; };
	static GLShapeIndexed MakeCube( CUBE_DESC& description );


	struct QUAD_DESC{ Math::Vector2 dimensions; };
	static GLShape MakeQuad( QUAD_DESC& description );

	struct PLANE_GRID_DESC{ Math::Vector2 size; Math::Vector2 layers; Math::Vector3 center; };
	static GLColorShape MakePlaneGrid( PLANE_GRID_DESC description );

};

