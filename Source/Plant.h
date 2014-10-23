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


	GLObject branchesObject;
	GLObject leavesObject;

	public:
	std::vector<Component> components;		// Tree components containing its hiearchycal info
	uint leavesIndex;

	static PlantSystem* system;



	//std::vector<PlantVertex> staticData;	//Represents the tree in its state of equilibrium
	//std::vector<PlantVertex> dynamicData;	//Represents the tree after being modified by external factors such as wind


	uint bufferIndex;
	uint bufferLength;
	//GLuint	staticDataBuffer;

	std::vector<Math::Vector3> locations;

	//Creates the tree structure of Components and the buffer that will be used on the GPU
	void Create( );

	void UpdateObject( PlantVertex* const vertices );
	
	
	enum DrawType { DRAW_BRANCHES, DRAW_LEAVES };
	void Draw( DrawType drawtype );

	enum TransformType { BRANCH_STRAIGHT, BRANCH_FORK, LEAF };
	PlantVertex Transform( const PlantVertex& vertex, uint index, uint count, TransformType type );


};

