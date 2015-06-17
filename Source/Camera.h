#pragma once

#include "Math.h"

class Camera 
{
	Math::Matrix44 worldMatrix;
	
	public:
	
	Math::Vector3 velocity;
	Math::Vector2 angularVelocity; // (yaw,pitch)


	Camera();

	void MoveCamera();

	Math::Matrix44 MakeViewMatrix( ) const;

	Math::Vector3 GetPosition() const;
	Math::Vector3 GetForward() const;
};

