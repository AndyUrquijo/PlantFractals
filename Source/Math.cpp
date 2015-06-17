/*
.	Author:		Andres Urquijo
.	Filename:	Math.cpp
*/
#include "Math.h"
#include <memory>
#include <cstdlib>

#include <algorithm>


namespace Math
{
	float UnitRand( )
	{
		return float( rand( ) % RAND_MAX ) / ( RAND_MAX - 1 );
	}

	float RangeRand( float min, float max )
	{
		return UnitRand( )*( max - min ) + min;
	}

	float Clamp( float value, float min, float max )
	{
		return min > value ? min : max < value ? max : value;
	}

	Vector2::Vector2( )
	{
		x = 0;
		y = 0;
	}

	Vector2::Vector2( float x, float y )
	{
		this->x = x;
		this->y = y;
	}

	Vector2::Vector2( const Vector2& copy )
	{
		memcpy( elm, copy.elm, sizeof elm );
	}

	Vector3::Vector3( const Vector4& casted )
	{
		memcpy( elm, casted.elm, sizeof elm );
	}

	Vector2::~Vector2( void ){}

	Vector2& Vector2::operator=( const Vector2& vector )
	{
		memcpy( elm, vector.elm, sizeof elm );
		return *this;
	}



	Vector2& Vector2::operator+=( const Vector2& vector )
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}
	Vector2& Vector2::operator-=( const Vector2& vector )
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}
	Vector2& Vector2::operator*=( float scalar )
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}


	// --- Static functions ---

	Vector2 Vector2::UnitVector( float theta )
	{
		return Vector2( cos( theta ), sin( theta ) );
	}

	// --- Global functions ---

	Vector2 operator*( float scalar, const Vector2& vector )
	{
		return Vector2( vector.x * scalar, vector.y * scalar );
	}


	// ---------------
	// --- Vector3 ---
	// ---------------


	Vector3::Vector3( )
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3::Vector3( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3::Vector3( const Vector3& copy )
	{
		memcpy( elm, copy.elm, sizeof elm );
	}

	Vector3::~Vector3( void ){}

	Vector3& Vector3::operator=( const Vector3& vector )
	{
		memcpy( elm, vector.elm, sizeof elm );
		return *this;
	}
	Vector3& Vector3::operator+=( const Vector3& vector )
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}
	Vector3& Vector3::operator-=( const Vector3& vector )
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}
	Vector3& Vector3::operator*=( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}




	Vector3 Vector3::Cross( const Vector3& vector ) const
	{
		const Vector3& a = *this, b = vector;
		return Vector3( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x );
	}

	Vector3 Vector3::Rotate( float angle, const Vector3& unitAxis ) const
	{
		float cos_a = cos( angle ), sin_a = sin( angle );
		const Vector3& v = *this, w = unitAxis;

		// Rodrigues rotation formula
		return v*cos_a + ( v.Cross( w ) )*sin_a + w*( v*w )*( 1 - cos_a );
	}


	// --- Static functions ---

	Vector3 Vector3::UnitVector( float psi, float theta )
	{
		return Vector3( sin( psi )*cos( theta ), sin( psi )*sin( theta ), cos( psi ) );
	}

	Vector3 Vector3::RandomRotation( const Vector3& vector,
									 float minTheta, float maxTheta,
									 float minPhi, float maxPhi )
	{
		//Vector3 xAxis, yAxis;
		//xAxis = { 1, 0, 0 };
		//yAxis = { 0, 1, 0 };
		//Vector3& perpendicular = abs( xAxis*vector ) > abs( yAxis*vector ) ? yAxis : xAxis; //choose the less colinear axis
#define ARBITRARY {0.57735026f, 0.57735026f, 0.57735026f}
		Vector3 perpendicular = ARBITRARY;
		perpendicular = vector.Cross( perpendicular );
		float theta = Math::UnitRand( )*( maxTheta - minTheta ) + minTheta; // Polar angle or angle from cenit (original direction)
		perpendicular = perpendicular.Rotate( theta, vector.Normalize( ) );
		float phi = Math::UnitRand( )*( maxPhi - minPhi ) + minPhi; // Azimuthal angle or angle in the plane perpendicular to the original vector
		return vector.Rotate( phi, perpendicular );
	}

	Vector3 Vector3::Randomize( const Vector3& min, const Vector3& max )
	{
		Vector3 random;
		random.x= RangeRand(min.x, max.x);
		random.y= RangeRand(min.y, max.y);
		random.z= RangeRand(min.z, max.z);
		return random;
	}
	Vector3 Vector3::Randomize( const Vector2& xRange, const Vector2& yRange, const Vector2& zRange )
	{
		return Randomize( { xRange.x, yRange.x, zRange.x }, { xRange.y, yRange.y, zRange.y } );
	}

	// --- Global functions ---

	Vector3 operator*( float scalar, const Vector3& vector )
	{
		return Vector3( vector.x * scalar, vector.y * scalar, vector.z * scalar );
	}






	// ---------------
	// --- Vector4 ---
	// ---------------


	Vector4::Vector4( )
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vector4::Vector4( float x, float y, float z, float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4::Vector4( const Vector4& copy )
	{
		memcpy( elm, copy.elm, sizeof elm );
	}

	Vector4::Vector4( const Vector3& xyz, float w )
	{
		memcpy( elm, xyz.elm, sizeof xyz.elm );
		this->w = w;
	}

	Vector4::~Vector4( void ){}

	Vector4& Vector4::operator=( const Vector4& vector )
	{
		memcpy( elm, vector.elm, sizeof elm );
		return *this;
	}
	Vector4& Vector4::operator+=( const Vector4& vector )
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}
	Vector4& Vector4::operator-=( const Vector4& vector )
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}
	Vector4& Vector4::operator*=( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vector4 Vector4::operator*( const Matrix44& matrix ) const
	{
		Vector4	prd;
		for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			prd.elm[i] += matrix.m[i][j] * this->elm[j];
		return prd;
	}


	// --- Static functions ---

	Vector4 Vector4::UnitVector( float psi, float theta )
	{
		return Vector4( sin( psi )*cos( theta ), sin( psi )*sin( theta ), cos( psi ), 0.0f );
	}


	// --- Global functions ---

	Vector4 operator*( float scalar, const Vector4& vector )
	{
		return Vector4( vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar );
	}

	// ----------------
	// --- Matrix44 ---
	// ----------------

	Matrix44::Matrix44( )
	{
		memset( this, 0, sizeof elm );
	}


	Matrix44::Matrix44( const float* elm )
	{
		memcpy( this->elm, elm, sizeof this->elm );
	}

	Matrix44::Matrix44(
		float a00, float a10, float a20, float a30,
		float a01, float a11, float a21, float a31,
		float a02, float a12, float a22, float a32,
		float a03, float a13, float a23, float a33 )
	{
		float elm[16] = {
			a00, a10, a20, a30,
			a01, a11, a21, a31,
			a02, a12, a22, a32,
			a03, a13, a23, a33
		};
		memcpy( this->elm, elm, sizeof elm );
	}

	Matrix44 Matrix44::operator+( const Matrix44& matrix ) const
	{
		Matrix44 sum;
		for ( int i = 0; i < 16; i++ )
			sum.elm[i] = this->elm[i] + matrix.elm[i];
		return sum;
	}
	Matrix44 Matrix44::operator-( const Matrix44& matrix ) const
	{
		Matrix44 sub;
		for ( int i = 0; i < 16; i++ )
			sub.elm[i] = this->elm[i] - matrix.elm[i];
		return sub;
	}
	Matrix44 Matrix44::operator*( const Matrix44& matrix ) const
	{
		Matrix44 prd = { };
		for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
		for ( int s = 0; s < 4; s++ )
			prd.m[i][j] += this->m[i][s] * matrix.m[s][j];
		return prd;
	}

	Vector4 Matrix44::operator*( const Vector4& vector ) const
	{
		Vector4	prd;
		for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			prd.elm[i] += this->m[i][j] * vector.elm[j];
		return prd;
	}

	Matrix44 Matrix44::operator*( float scalar ) const
	{
		Matrix44 prd;
		for ( int i = 0; i < 16; i++ )
			prd.elm[i] = this->elm[i] * scalar;
		return prd;
	}


	Matrix44& Matrix44::operator*=( const Matrix44& matrix )
	{
		return *this = *this*matrix;
	}

	Matrix44 Matrix44::Transpose( ) const
	{
		Matrix44 transpose;
		for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
			transpose.m[i][j] = this->m[j][i];
		return transpose;
	}




float invf(int i,int j,const Matrix44& mat)
{

    int o = 2+(j-i);

    i += 4+o;
    j += 4-o;

    #define e(a,b) mat.elm[((j+(b))%4)*4 + ((i+(a))%4)]

    float inv =
     + e(+1,-1)*e(+0,+0)*e(-1,+1)
     + e(+1,+1)*e(+0,-1)*e(-1,+0)
     + e(-1,-1)*e(+1,+0)*e(+0,+1)
     - e(-1,-1)*e(+0,+0)*e(+1,+1)
     - e(-1,+1)*e(+0,-1)*e(+1,+0)
     - e(+1,-1)*e(-1,+0)*e(+0,+1);

    return (o%2)?inv : -inv;

    #undef e
}

	// Thanks to 'tea' at stackoverflow
Matrix44 inverseMatrix4x4(Matrix44 mat)
{

    Matrix44 inv;

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            inv.m[i][j] = invf(i,j,mat);

    double D = 0;

    for(int k=0;k<4;k++) 
		D += mat.m[k][0] * inv.m[0][k];

    if (D == 0)
		return Matrix44::MakeZeros();

    float invD = float(1.0 / D);

	mat = inv*invD;

    return mat;
}
	Matrix44 Matrix44::Inverse( ) const
	{
		return inverseMatrix4x4( *this );
	}

	// --- Static functions ---

	Matrix44 Matrix44::MakeIdentity( )
	{
		Matrix44 identity =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		return identity;
	}

	Matrix44 Matrix44::MakeZeros( )
	{
		Matrix44 zeros;
		memset( &zeros, 0, sizeof zeros.elm );
		return zeros;
	}

	Matrix44 Matrix44::MakeTranslation( const Vector3& translation )
	{
		Matrix44 translationMatrix = MakeIdentity( );
		for ( int i = 0; i < 3; i++ )
			translationMatrix.m[3][i] = translation.elm[i];
		return translationMatrix;
	}

	Matrix44 Matrix44::MakeXRotation( float angle )
	{
		float s = sin( angle ), c = cos( angle );
		Matrix44 xRotation =
		{
			1, 0, 0, 0,
			0, c, s, 0,
			0, -s, c, 0,
			0, 0, 0, 1,
		};
		return xRotation;
	}

	Matrix44 Matrix44::MakeYRotation( float angle )
	{
		float s = sin( angle ), c = cos( angle );
		Matrix44 yRotation =
		{
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1,
		};
		return yRotation;
	}

	Matrix44 Matrix44::MakeZRotation( float angle )
	{
		float s = sin( angle ), c = cos( angle );
		Matrix44 zRotation =
		{
			c, s, 0, 0,
			-s,	c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		return zRotation;
	}

	Matrix44 Matrix44::MakeProjectionMatrix( float fov, float aspect, float zMin, float zMax )
	{
		//code adapted from directX
		Matrix44 proj;
		float    SinFov = sin( fov*0.5f );
		float    CosFov = cos( fov*0.5f );

		float Height = CosFov / SinFov;
		float Width = Height / aspect;
		float fRange = zMax / ( zMax - zMin );

		proj.m[0][0] = Width;
		proj.m[0][1] = 0.0f;
		proj.m[0][2] = 0.0f;
		proj.m[0][3] = 0.0f;

		proj.m[1][0] = 0.0f;
		proj.m[1][1] = Height;
		proj.m[1][2] = 0.0f;
		proj.m[1][3] = 0.0f;

		proj.m[2][0] = 0.0f;
		proj.m[2][1] = 0.0f;
		proj.m[2][2] = fRange;
		proj.m[2][3] = 1.0f;

		proj.m[3][0] = 0.0f;
		proj.m[3][1] = 0.0f;
		proj.m[3][2] = -fRange * zMin;
		proj.m[3][3] = 0.0f;

		return proj;
	}





	Short2::Short2( )
	{
		x = 0;
		y = 0;
	}
	Short2::Short2( short x, short y )
	{
		this->x = x;
		this->y = y;
	}
	Short2::Short2( const Short2& copy )
	{
		this->x = copy.x;
		this->y = copy.y;
	}
	Short2& Short2::operator=( const Short2& short2 )
	{
		this->x = short2.x;
		this->y = short2.y;
		return *this;
	}

}