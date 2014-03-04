#pragma once

#include "Math.h"

class Camera 
{
	public:
	Math::Vector3 orientation;
	Math::Vector3 position;
	
	Math::Vector3 velocity;
	Math::Vector3 angularVelocity;


	Camera();

	void MoveCamera();

	Math::Matrix44 MakeViewMatrix( ) const;
};

