#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

uniform float TIME;

layout(std140, binding = 0) buffer staticDataBuffer
{
      vec4 positions[];
} staticData;

layout(std140, binding = 1) buffer dynamicDataBuffer
{
      vec4 positions[];
} dynamicData;


layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
 
vec3 rotate( vec3 v, float angle, const vec3 w )
{
	float cos_a = cos( angle ), sin_a = sin( angle );
	return v*cos_a + cross(v,w )*sin_a + w*( dot(v,w) )*( 1 - cos_a );	// Rodriges rotation formula
}

const vec3 wind = vec3(-1,0,0)*0.5;

// Declare main program function which is executed once
// glDispatchCompute is called from the application.
void main()
{
	uint id = gl_GlobalInvocationID.x;
	
	vec3 Ro = staticData.positions[id].xyz;
	vec3 R;

	float r = length(Ro);

	float w = length(cross(Ro,wind));
	vec3 Wo = Ro + wind*sqrt(w) ; // equilibrium position with wind
	Wo = normalize(Wo)*r;

	float Ao = acos( dot( Ro, Wo ) / ( r*r ) ); //angle between Wo and Ro

	vec3 n = normalize( cross( Ro, Wo ) );	//unit vector normal to Rp and Wo
	float a = 10.0 / r;		//oscilation frequency

	R = rotate(Wo, 0.3*Ao*sin( a*TIME ), n );

	dynamicData.positions[id] = vec4(R,0);
}