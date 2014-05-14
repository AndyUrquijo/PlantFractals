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
#define CAMERA_ACCELERATION 400.0f
#define CAMERA_FRICTION 300.0f
#define CAMERA_TOP_ANGULAR_SPEED 20.0f
#define CAMERA_TOP_ANGULAR_SPEEDX 15.0f
#define CAMERA_TOP_ANGULAR_SPEED_MOUSE 5.0f
#define CAMERA_ANGULAR_ACCELERATION 4.0f
#define CAMERA_FRICTION_TORQUE 3.0f

	static Clock clock;
	float dt = (float) clock.Tick( );

	// Translation

	Vector3 dV( 0, 0, 0 );

	if ( GetAsyncKeyState( 'W' ) )		dV.z--;
	if ( GetAsyncKeyState( 'S' ) )		dV.z++;

	if ( GetAsyncKeyState( 'A' ) )		dV.x++;
	if ( GetAsyncKeyState( 'D' ) )		dV.x--;

	if ( GetAsyncKeyState( 'Q' ) )		dV.y++;
	if ( GetAsyncKeyState( 'E' ) )		dV.y--;

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

	Vector3 worldV = Vector3( Vector4( velocity, 0 )*Matrix44::MakeYRotation( orientation.y )*Matrix44::MakeXRotation( orientation.x ) );
	position += worldV*dt;



	// Rotation

	Vector3 dW( 0, 0, 0 );
	POINT curr; // current mouse location
	GetPhysicalCursorPos( &curr ); // determine new location
	static POINT prev = curr; // previous mouse location
	if ( GetAsyncKeyState( VK_RBUTTON ) )
	{
		dW.y = float( curr.x - prev.x ) / 50.0f;
		dW.x = float( curr.y - prev.y ) / 50.0f;

		if ( !dW.IsZero( ) )
			angularVelocity = dW*CAMERA_TOP_ANGULAR_SPEED_MOUSE;
	}
	else
	{
		if ( GetAsyncKeyState( VK_LEFT ) ) 		dW.y--;
		if ( GetAsyncKeyState( VK_RIGHT ) )		dW.y++;
		if ( GetAsyncKeyState( VK_UP ) )		dW.x--;
		if ( GetAsyncKeyState( VK_DOWN ) )		dW.x++;

		if ( !dW.IsZero( ) )
			angularVelocity += dW.Normalize( )*CAMERA_ANGULAR_ACCELERATION*dt;
	}
	prev = curr;

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

	orientation += angularVelocity*dt;
}

Matrix44 Camera::MakeViewMatrix( ) const
{
	Matrix44 transform = Matrix44::MakeIdentity( );
	transform *= Matrix44::MakeTranslation( position*( 1 ) );
	transform *= Matrix44::MakeZRotation( orientation.z*( 1 ) );
	transform *= Matrix44::MakeYRotation( orientation.y*( 1 ) );
	transform *= Matrix44::MakeXRotation( orientation.x*( 1 ) );
	//transform = transform.Transpose();
	return transform;
}

Vector3 Camera::GetForward( ) const
{
	return Vector3( Vector4( position, 0 )*Matrix44::MakeYRotation( orientation.y )*Matrix44::MakeXRotation( orientation.x ) );
}
