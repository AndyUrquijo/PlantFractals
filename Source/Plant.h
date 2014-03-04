#pragma once

#include "GLObject.h"
#include "Math.h"
#include <vector>
#include "GLSharedTypes.h"

class Plant
{
	struct Component
	{
		Math::Vector3 displacement;
		GLushort parentIndex;
	};


	GLObject object;

	std::vector<Component> components;

	public:
	
	Math::Vector3 location;
	
	Plant( );
	~Plant( );

	void Grow( );
	void InitializeObject( );

	void Draw();
};

