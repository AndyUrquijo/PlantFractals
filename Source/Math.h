/*
.	Author:		Andres Urquijo
.	Filename:	Math.h
*/
#pragma once

#include <cmath>

#undef Rectangle

namespace Math
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Matrix44;
	struct Rectangle;

	struct Short2;

	static const float PI =	3.14159265359f;	// The number pi.

	// Returns a random number in the range [0, 1)
	float UnitRand( );
	// Returns a random number in the range [min, max)
	float RangeRand( float min, float max );
	// Clamps a value to the range [min, max]
	float Clamp( float value, float min, float max );

	
	//-----------------
	// Vector2
	//-----------------

	// Two-Dimensional vector with float precision.
	struct Vector2
	{
		union
		{
			struct
			{
				float x;
				float y;
			};
			float elm[2];
		};

		Vector2( );
		Vector2( float x, float y );
		Vector2( const Vector2& copy );
		~Vector2( void );

		Vector2 operator+( const Vector2& vector ) const	{ return Vector2( x + vector.x, y + vector.y ); }
		Vector2 operator-( const Vector2& vector ) const	{ return Vector2( x - vector.x, y - vector.y ); }
		Vector2 operator*( float scalar ) const				{ return Vector2( x * scalar, y * scalar ); }
		float operator*( const Vector2& vector ) const		{ return x*vector.x + y*vector.y; }

		Vector2& operator=( const Vector2& vector );
		Vector2& operator+=( const Vector2& vector );
		Vector2& operator-=( const Vector2& vector );
		Vector2& operator*=( float scalar );

		bool operator==( const Vector2& vector ) const		{ return this->x == vector.x && this->y == vector.y; }
		bool operator!=( const Vector2& vector ) const		{ return this->x == vector.x || this->y != vector.y; }

		//The vector's Magnitude.
		float Length( ) const		{ return sqrtf( x*x + y*y ); }

		//The vector's Magnitude squared.
		float LengthSq( ) const		{ return x*x + y*y; }

		Vector2 Normalize( ) const		{ return *this*( 1.0f / Length( ) ); }


		//Element by element multiplication
		Vector2 ElementMultiply( const Vector2& vector ) const	{ return Vector2( x*vector.x, y*vector.y ); }
		Vector2 ElementMultiply( float x, float y ) const			{ return Vector2( this->x*x, this->y*y ); }

		bool IsZero( float eps = 0.0f ) const		{ return fabsf( x ) <= eps && fabsf( y ) <= eps; }

		// Constructs a unit vector with an angle in polar coordinates.
		static Vector2 UnitVector( float theta );
	};

	Vector2 operator*( float scalar, const Vector2& vector );


	//-----------------
	// Vector3
	//-----------------

	// Three-Dimensional vector with float precision.
	struct Vector3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
			struct
			{
				float r;
				float g;
				float b;
			};
			float elm[3];
		};

		Vector3( );
		Vector3( float x, float y, float z );
		Vector3( const Vector3& copy );
		explicit Vector3( const Vector4& casted );
		~Vector3( void );

		Vector3 operator+( const Vector3& vector ) const	{ return Vector3( x + vector.x, y + vector.y, z + vector.z ); }
		Vector3 operator-( const Vector3& vector ) const	{ return Vector3( x - vector.x, y - vector.y, z - vector.z ); }
		Vector3 operator*( float scalar ) const				{ return Vector3( x * scalar, y * scalar, z * scalar ); }
		float operator*( const Vector3& vector ) const		{ return x*vector.x + y*vector.y + z*vector.z; }

		Vector3& operator=( const Vector3& vector );
		Vector3& operator+=( const Vector3& vector );
		Vector3& operator-=( const Vector3& vector );
		Vector3& operator*=( float scalar );

		bool operator==( const Vector3& vector ) const		{ return this->x == vector.x && this->y == vector.y && this->z == vector.z; }
		bool operator!=( const Vector3& vector ) const		{ return this->x == vector.x || this->y != vector.y || this->z != vector.z; }

		//The vector's Magnitude.
		float Length( ) const			{ return sqrtf( x*x + y*y + z*z ); }

		//The vector's Magnitude squared.
		float LengthSq( ) const			{ return x*x + y*y + z*z; }

		Vector3 Normalize( ) const		{ return *this*( 1.0f / Length( ) ); }


		//Element by element multiplication
		Vector3 ElementMultiply( const Vector3& vector ) const	{ return Vector3( x*vector.x, y*vector.y, z*vector.z ); }
		Vector3 ElementMultiply( float x, float y, float z ) const			{ return Vector3( this->x*x, this->y*y, this->z*z ); }

		bool IsZero( ) const				{ return  x == 0.0f && y == 0.0f && z == 0.0f; }
		bool IsZero( float eps ) const		{ return fabsf( x ) <= eps && fabsf( x ) <= eps && fabsf( z ) <= eps; }

		Vector3 Cross( const Vector3& vector ) const;


		Vector3 Rotate( float angle, const Vector3& unitAxis ) const;

		// Constructs a unit vector with two angles in spherical coordinates.
		static Vector3 UnitVector( float psi, float theta );

		// Rotates the vector in a random direction
		static Vector3 RandomRotation( const Vector3& vector,
									   float minTheta, float maxTheta,
									   float minPhi, float maxPhi );

		static Vector3 Randomize( const Vector3& min, const Vector3& max );
		static Vector3 Randomize( const Vector2& xRange, const Vector2& yRange, const Vector2& zRange );

		static Vector3 RandomOrthonormal( const Vector3& normal );

		static const Vector3 UP;
		static const Vector3 RIGHT;
		static const Vector3 FORWARD;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 BACKWARD;
	};

	Vector3 operator*( float scalar, const Vector3& vector );



	//-----------------
	// Vector4
	//-----------------

	// Four-Dimensional vector with float precision.
	struct Vector4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
			float elm[4];
		};

		Vector4( );
		Vector4( float x, float y, float z, float w );
		Vector4( const Vector4& copy );
		Vector4( const Vector3& xyz, float w );
		~Vector4( void );

		Vector4 operator+( const Vector4& vector ) const	{ return Vector4( x + vector.x, y + vector.y, z + vector.z, w + vector.w ); }
		Vector4 operator-( const Vector4& vector ) const	{ return Vector4( x - vector.x, y - vector.y, z - vector.z, w - vector.w ); }
		Vector4 operator*( float scalar ) const				{ return Vector4( x * scalar, y * scalar, z * scalar, w * scalar ); }
		float operator*( const Vector4& vector ) const		{ return x*vector.x + y*vector.y + z*vector.z + w*vector.w; }

		Vector4& operator=( const Vector4& vector );
		Vector4& operator+=( const Vector4& vector );
		Vector4& operator-=( const Vector4& vector );
		Vector4& operator*=( float scalar );

		bool operator==( const Vector4& vector ) const		{ return this->x == vector.x && this->y == vector.y && this->z == vector.z && this->w == vector.w; }
		bool operator!=( const Vector4& vector ) const		{ return this->x == vector.x || this->y != vector.y || this->z != vector.z || this->w != vector.w; }

		Vector4 operator*( const Matrix44& matrix ) const;

		//The vector's Magnitude.
		float Length( ) const		{ return sqrtf( x*x + y*y + z*z ); }

		//The vector's Magnitude squared.
		float LengthSq( ) const		{ return x*x + y*y + z*z; }

		//Element by element multiplication
		Vector4 ElementMultiply( const Vector4& vector ) const				{ return Vector4( x*vector.x, y*vector.y, z*vector.z, w*vector.w ); }
		Vector4 ElementMultiply( float x, float y, float z, float w ) const	{ return Vector4( this->x*x, this->y*y, this->z*z, this->w*w ); }

		bool IsZero( float eps = 0.0f ) const		{ return fabsf( x ) <= eps && fabsf( x ) <= eps && fabsf( z ) <= eps; }

		// Constructs a unit vector with two angles in spherical coordinates.
		static Vector4 UnitVector( float psi, float theta );
	};

	Vector4 operator*( float scalar, const Vector4& vector );


	// Matrix44
	struct Matrix44
	{
		union
		{
			struct
			{
				float
				xx, xy, xz, xw,
				yx, yy, yz, yw,
				zx, zy, zz, zw,
				wx, wy, wz, ww;
			};
			struct
			{
			Vector3 xAxis;
			float	xPadding;
			Vector3 yAxis;
			float	yPadding;
			Vector3 zAxis;
			float	zPadding;
			Vector3 wAxis;
			float	wPadding;
			};
			float m[4][4];
			float elm[16];
		};


		Matrix44( );	// Initialize with zero values.

		// Constructs a matrix from a pointer to an array containing 16 elements. 
		// If the array a size of at least 16 it could lead to undesired results.
		Matrix44( const float* elm );
		Matrix44(
			float a00, float a10, float a20, float a30,
			float a01, float a11, float a21, float a31,
			float a02, float a12, float a22, float a32,
			float a03, float a13, float a23, float a33 );

		Matrix44 operator+( const Matrix44& matrix ) const;
		Matrix44 operator-( const Matrix44& matrix ) const;
		Matrix44 operator*( const Matrix44& matrix ) const;
		Vector4 operator*( const Vector4& vector ) const;
		Matrix44 operator*( float scalar ) const;

		Matrix44& operator*=( const Matrix44& matrix );

		Matrix44 Transpose( ) const;
		Matrix44 Inverse( ) const; // NOTE: Function not yet functional

		static Matrix44 MakeIdentity( );
		static Matrix44 MakeZeros( );
		static Matrix44 MakeTranslation( const Vector3& translation );
		static Matrix44 MakeXRotation( float angle );
		static Matrix44 MakeYRotation( float angle );
		static Matrix44 MakeZRotation( float angle );
		static Matrix44 MakeProjectionMatrix( float fov, float aspect, float zMin, float zMax );
	};



	struct Rectangle
	{
		union
		{
			struct
			{
				float left;
				float top;
				float right;
				float bottom;
			};
			struct
			{
				Vector2 origin;
				Vector2 size;
			};
			float elm[4];
		};

	};


	struct Short2
	{
		union
		{
			struct
			{
				short x;
				short y;
			};
			short elm[2];
		};

		Short2( );
		Short2( short x, short y );
		Short2( const Short2& copy );
		Short2& operator=( const Short2& short2 );
	};

}