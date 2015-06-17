#include "Camera.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Clock.h"

using namespace Math;

Camera::Camera( )
{
	worldMatrix = Math::Matrix44::MakeTranslation({ 0, 15, 30 })*Matrix44::MakeYRotation(PI);
	velocity = { 0, 0, 0 };
}


void Camera::MoveCamera( )
{
#define CAMERA_TOP_SPEED 150.0f
#define CAMERA_ACCELERATION 400.0f
#define CAMERA_FRICTION 300.0f
#define CAMERA_TOP_ANGULAR_SPEED 20.0f
#define CAMERA_TOP_ANGULAR_SPEED_MOUSE 5.0f
#define CAMERA_ANGULAR_ACCELERATION 1.0f
#define CAMERA_FRICTION_TORQUE 0.5f

	static Clock clock;
	float dt = (float) clock.Tick( );

	// Translation

	Vector3 dV( 0, 0, 0 );

	if ( GetAsyncKeyState( 'W' ) )		dV.z++;
	if ( GetAsyncKeyState( 'S' ) )		dV.z--;

	if ( GetAsyncKeyState( 'A' ) )		dV.x--;
	if ( GetAsyncKeyState( 'D' ) )		dV.x++;

	if ( GetAsyncKeyState( 'Q' ) )		dV.y--;
	if ( GetAsyncKeyState( 'E' ) )		dV.y++;

	if ( !dV.IsZero( ) )
		velocity += dV.Normalize( )*CAMERA_ACCELERATION*dt;

	velocity *= min( 1.0f, sqrtf( CAMERA_TOP_SPEED*CAMERA_TOP_SPEED / velocity.LengthSq( ) ) );

	{
		float v = velocity.Length( );
		float fr = CAMERA_FRICTION*dt;
		if ( v <= fr )
			velocity = { 0, 0, 0 };
		else
			velocity -= velocity*( fr / v );
	}


	worldMatrix.wAxis += worldMatrix.xAxis * velocity.x * dt;
	worldMatrix.wAxis += worldMatrix.yAxis * velocity.y * dt;
	worldMatrix.wAxis += worldMatrix.zAxis * velocity.z * dt;


	// Rotation

	Vector2 dW( 0, 0 );
	POINT curr; // current mouse location
	GetPhysicalCursorPos( &curr ); // determine new location
	static POINT prev = curr; // previous mouse location
	if ( GetAsyncKeyState( VK_RBUTTON ) )
	{
		dW.y =  float( curr.x - prev.x ) / 50.0f;
		dW.x = -float( curr.y - prev.y ) / 50.0f;

		if ( !dW.IsZero( ) )
			angularVelocity = dW*CAMERA_TOP_ANGULAR_SPEED_MOUSE;
	}
	else
	{
		if ( GetAsyncKeyState( VK_LEFT ) ) 		dW.y++;
		if ( GetAsyncKeyState( VK_RIGHT ) )		dW.y--;
		if ( GetAsyncKeyState( VK_UP ) )		dW.x--;
		if ( GetAsyncKeyState( VK_DOWN ) )		dW.x++;
		
		if ( !dW.IsZero( ) )
			angularVelocity += dW.Normalize( )*CAMERA_ANGULAR_ACCELERATION*dt;
	}
	prev = curr;

	angularVelocity.x = Clamp( angularVelocity.x, -CAMERA_TOP_ANGULAR_SPEED, CAMERA_TOP_ANGULAR_SPEED );
	angularVelocity.y = Clamp( angularVelocity.y, -CAMERA_TOP_ANGULAR_SPEED, CAMERA_TOP_ANGULAR_SPEED );

	float frictionTorque = CAMERA_FRICTION_TORQUE*dt;
	for ( int i = 0; i < 2; i++ )
	{
		if ( angularVelocity.elm[i] >  frictionTorque ) angularVelocity.elm[i] -= frictionTorque;
		else if ( angularVelocity.elm[i] < -frictionTorque ) angularVelocity.elm[i] += frictionTorque;
		else angularVelocity.elm[i] = 0;
	}


	// World Y rotation
	Vector3 pos = worldMatrix.wAxis;
	worldMatrix.wAxis = { 0, 0, 0 };
	worldMatrix = worldMatrix*Matrix44::MakeYRotation(angularVelocity.y); // Local X rotation and world Y rotation
	worldMatrix.wAxis = pos;


	worldMatrix = Matrix44::MakeXRotation(angularVelocity.x)*worldMatrix;

}

Matrix44 Camera::MakeViewMatrix( ) const
{
	Matrix44 rotationMatrixInv = worldMatrix;
	rotationMatrixInv.wAxis = { 0, 0, 0 };
	rotationMatrixInv = rotationMatrixInv.Transpose();

	Matrix44 translationMatrixInv = Matrix44::MakeTranslation( -1*worldMatrix.wAxis );

	Matrix44 viewMatrix = translationMatrixInv*rotationMatrixInv;
	
	return viewMatrix;
}

Math::Vector3 Camera::GetPosition( ) const
{
	return worldMatrix.wAxis;
}

Vector3 Camera::GetForward( ) const
{
	return worldMatrix.zAxis;
}
