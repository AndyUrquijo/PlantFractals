#ifdef __cplusplus

#pragma once
#include <GL\glew.h>
#include "Math.h"
using namespace Math;

typedef float		FLOAT;
typedef Vector2		FLOAT2;
typedef Vector3		FLOAT3;
typedef Vector4		FLOAT4;
typedef Matrix44	FLOAT4X4;

typedef GLushort	ushort;	
typedef GLuint		uint;	

typedef const char* PCSTR;

#else

typedef float			FLOAT;
typedef float2			FLOAT2;
typedef float3			FLOAT3;
typedef float4			FLOAT4;
typedef float4x4		FLOAT4X4;

#define _POSITION :POSITION
#define _COLOR :COLOR
#define _TEXCOORD :TEXCOORD
#define _NORMAL :NORMAL

#endif

enum GLVertexAttribute
{
	VERTEX_POSITION,
	VERTEX_COLOR,
	VERTEX_TEXTURE,
	VERTEX_TEXTURE0 = VERTEX_TEXTURE,
	VERTEX_NORMAL,
};
enum GLVertexAttributeBit
{
	VERTEX_POSITION_BIT = 1,
	VERTEX_COLOR_BIT = 2,
	VERTEX_TEXTURE_BIT = 4, VERTEX_TEXTURE0_BIT = VERTEX_TEXTURE_BIT,
	VERTEX_NORMAL_BIT = 8,
};


struct VertexTN
{
	FLOAT3 position;
	FLOAT2 texCoord;
	FLOAT3 normal;
};

struct VertexC
{
	FLOAT3 position;
	FLOAT4 color;
};

struct OBJECT_DATA
{
	FLOAT4X4 worldMatrix;
};

struct WORLD_DATA
{
	FLOAT4X4	viewMatrix;
	FLOAT4X4	projectionMatrix;

	FLOAT		time;
	FLOAT		dt;
	FLOAT2		padding;
};


struct AMBIENT_LIGHT
{
	FLOAT4	color;
};

struct POINT_LIGHT
{
	FLOAT4	color;
	FLOAT3	position;
	FLOAT		radius;
};

struct SPOT_LIGHT
{
	FLOAT4	color;
	FLOAT3	position;
	FLOAT 	radius;
	FLOAT3	direction;
	FLOAT	innerRatio;
	FLOAT	outerRatio;
	FLOAT3	padding;
};

struct DIRECTIONAL_LIGHT
{
	FLOAT4	color;
	FLOAT3	direction;
	FLOAT	padding;
};

struct LIGHT_DATA
{
	AMBIENT_LIGHT		ambientLight;
	POINT_LIGHT			pointLight;
	DIRECTIONAL_LIGHT	directionalLight;
	SPOT_LIGHT			spotLight;
};


// Buffer for per-frame updates

struct CBUFFER0
{
	WORLD_DATA worldData;
	LIGHT_DATA lightData;
};

// Buffer for per-draw updates

struct CBUFFER1
{
	OBJECT_DATA objectData;
};
