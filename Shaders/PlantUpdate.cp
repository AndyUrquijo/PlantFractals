#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

uniform float TIME;

struct PlantVertex
{
	vec3 position;
	float level;
	vec3 normal;
	float delay;
};

layout(std140, binding = 0) buffer staticDataBuffer
{
      PlantVertex vertices[];
} staticData;

layout(std140, binding = 1) buffer dynamicDataBuffer
{
      PlantVertex vertices[];
} dynamicData;


layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
 
vec3 rotate( vec3 v, float angle, const vec3 w )
{
	float cos_a = cos( angle ), sin_a = sin( angle );
	return v*cos_a + cross(v,w )*sin_a + w*( dot(v,w) )*( 1 - cos_a );	// Rodriges rotation formula
}

const vec3 wind = vec3(-1,0,0)*0.3;

void main()
{
	uint id = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y*100;
	
	float level = staticData.vertices[id].level;
	float delay = staticData.vertices[id].delay;
	
	vec3 Ro = staticData.vertices[id].position;
	vec3 No = staticData.vertices[id].normal;
	vec3 R;
	vec3 N;



	float r = length(Ro);

	float Wf = length(cross(Ro,wind))*level* (float(level>1.0));
	vec3 Wo = Ro + wind*sqrt(Wf) ; // equilibrium position with wind
	Wo = normalize(Wo)*r;

	float Ao = acos( dot( Ro, Wo ) / ( r*r ) ); //angle between Wo and Ro

	vec3 n = normalize( cross( Ro, Wo ) );	//unit vector normal to Rp and Wo
	float w = 10.0 / r;		//oscilation frequency


	float a = 0.3*Ao*sin( delay + w*TIME );
	R = rotate(Wo, a, n );
	R = normalize(R)*r;

	vec3 pr = normalize( cross(R, Ro) );
	float ar = acos( dot(Ro,R) / ( r*r ) );
	N = rotate(No, a, pr);
	
	dynamicData.vertices[id].position = R;
	dynamicData.vertices[id].normal= N;
	dynamicData.vertices[id].level = level;
	dynamicData.vertices[id].delay = delay;
}