#pragma once

#include "Math.h"
#include "GLShape.h"

struct GLGeometry
{
	struct CUBE_DESC{ Math::Vector3 dimensions; };
	static GLShape MakeCube( CUBE_DESC& description );

};

