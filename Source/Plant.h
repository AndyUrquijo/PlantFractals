#pragma once

#include "GLObject.h"
#include "Math.h"
#include <vector>
#include "GLSharedTypes.h"

struct PlantVertex
{
	Vector3 position;	// contains the position in world space.
	float	level;		// the level in the tree (0 for the root) // the position in the branch (for leaves)
	Vector3 normal;		// a unit vector perpendicular to the branch's direction.
	float	delay;
};

class PlantSystem;

class Plant
{
	struct Component
	{
		GLushort parentIndex;
		GLushort firstChildIndex;
		GLushort childCount;
	};


	GLObject object;

	public:
	std::vector<Component> components;		// Tree components containing its hiearchycal info
	uint leavesIndex;

	static PlantSystem* system;



	//std::vector<PlantVertex> staticData;	//Represents the tree in its state of equilibrium
	//std::vector<PlantVertex> dynamicData;	//Represents the tree after being modified by external factors such as wind


	uint bufferIndex;
	uint bufferLength;
	//GLuint	staticDataBuffer;

	Math::Vector3 location;

	//Creates the tree structure of Components and the buffer that will be used on the GPU
	void Create( );

	void Update( float time );
	void UpdateWithCompute( );
	void ReadDynamicData( );

	void UpdateObject( );

	void Draw( );

	enum TransformType { KEEP_DIRECTION, EVEN_DISTRIBUTION, RANDOM_DISTRIBUTION };
	PlantVertex Transform( const PlantVertex& vertex, uint index, uint count, TransformType type );


};

