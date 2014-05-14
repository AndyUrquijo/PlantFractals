#pragma once

#include "GLObject.h"
#include "Math.h"
#include <vector>
#include "GLSharedTypes.h"

class Plant
{
	struct Component
	{
		float delay; 

		GLushort parentIndex;
		GLushort firstChildIndex;
		GLushort childCount;
	};


	GLObject object;

	std::vector<Component> components;		// Tree components containing its hiearchycal info

	std::vector<Vector4> staticPositions;	//Represents the displacement of the node, in an equilibrium state
	std::vector<Vector4> dynamicPositions;	//Represents the actual displacement of the node.


	public:
	
	 GLuint	staticPositionsBuffer;
	static GLuint	dynamicPositionsBuffer;

	Math::Vector3 location;

	int GetComponentCount( ) { return (int)components.size( ); }

	//Creates the tree structure of Components and the buffer that will be used on the GPU
	void Create( );

	void Update( );
	void ReadDynamicData();

	void UpdateObject( );

	void Draw();

	static float timeVal;
	static std::vector<Plant> plantArray;
	  void UpdateWithCompute();


};

