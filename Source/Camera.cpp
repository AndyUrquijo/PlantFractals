#include "Camera.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Clock.h"

using namespace Math;

Camera::Camera( )
{
	velocity = { 0, 0, 0 };
}


void Camera::MoveCamera( )
{
#define CAMERA_TOP_SPEED 150.0f
#define CAMERA_ACCELERATION 40.0f
#define CAMERA_FRICTION 30.0f
#define CAMERA_TOP_ANGULAR_SPEED 0.002f
#define CAMERA_TOP_ANGULAR_SPEEDX 0.001f
#define CAMERA_ANGULAR_ACCELERATION 0.4f
#define CAMERA_FRICTION_TORQUE 0.2f

	static Clock clock;
	float dt = (float)clock.Tick( );

	Vector3 dV( 0, 0, 0 );

	if ( GetAsyncKeyState( 'W' ) )
		dV.z++;
	if ( GetAsyncKeyState( 'S' ) )
		dV.z--;

	if ( GetAsyncKeyState( 'A' ) )
		dV.x++;
	if ( GetAsyncKeyState( 'D' ) )
		dV.x--;

	if ( GetAsyncKeyState( 'Q' ) )
		dV.y++;
	if ( GetAsyncKeyState( 'E' ) )
		dV.y--;

	if ( dV.LengthSq( ) > 0 )
	{
		dV = Vector3( ( Matrix44::MakeYRotation( orientation.y )*Matrix44::MakeXRotation( orientation.x ) )*Vector4( dV, 1 ) );
		float dv = CAMERA_ACCELERATION*dt;
		dV *= sqrtf( dv*dv / dV.LengthSq( ) );
		velocity += dV;
	}
	//velocity.x = Clamp( velocity.x, -CAMERA_TOP_SPEED, CAMERA_TOP_SPEED );
	//velocity.y = Clamp( velocity.y, -CAMERA_TOP_SPEED, CAMERA_TOP_SPEED );
	//velocity.z = Clamp( velocity.z, -CAMERA_TOP_SPEED, CAMERA_TOP_SPEED );

	velocity *= min( 1.0f, sqrtf( CAMERA_TOP_SPEED*CAMERA_TOP_SPEED / velocity.LengthSq( ) ) );

	{
		float v = velocity.Length( );
		float fr = CAMERA_FRICTION*dt;
		if ( v <= fr )
			velocity = { 0, 0, 0 };
		else
			velocity -= velocity*(fr/v);
	}

	position += velocity*dt;

	float dW = CAMERA_ANGULAR_ACCELERATION*dt;
	if ( GetAsyncKeyState( VK_LEFT ) )
		angularVelocity.y += dW;
	if ( GetAsyncKeyState( VK_RIGHT ) )
		angularVelocity.y -= dW;
	if ( GetAsyncKeyState( VK_UP ) )
		angularVelocity.x += dW;
	if ( GetAsyncKeyState( VK_DOWN ) )
		angularVelocity.x -= dW;

	angularVelocity.x = Clamp( angularVelocity.x, -CAMERA_TOP_ANGULAR_SPEEDX, CAMERA_TOP_ANGULAR_SPEEDX );
	angularVelocity.y = Clamp( angularVelocity.y, -CAMERA_TOP_ANGULAR_SPEED, CAMERA_TOP_ANGULAR_SPEED );
	angularVelocity.z = Clamp( angularVelocity.z, -CAMERA_TOP_ANGULAR_SPEED, CAMERA_TOP_ANGULAR_SPEED );

	float frictionTorque = CAMERA_FRICTION_TORQUE*dt;
	for ( int i = 0; i < 3; i++ )
	{
		if ( angularVelocity.elm[i] >  frictionTorque ) angularVelocity.elm[i] -= frictionTorque;
		else if ( angularVelocity.elm[i] < -frictionTorque ) angularVelocity.elm[i] += frictionTorque;
		else angularVelocity.elm[i] = 0;
	}

	orientation += angularVelocity;
}

Math::Matrix44 Camera::MakeViewMatrix( ) const
{
	Matrix44 transform = Matrix44::MakeTranslation( position*( -1 ) );
	transform = Matrix44::MakeZRotation( orientation.z*( -1 ) )*transform;
	transform = Matrix44::MakeYRotation( orientation.y*( -1 ) )*transform;
	transform = Matrix44::MakeXRotation( orientation.x*( -1 ) )*transform;
	return transform;
}