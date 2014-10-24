//Plant.vp
#version 110

uniform mat4 WORLD;
uniform mat4 VP;

layout(lines) in;
layout (triangle_strip, max_vertices=8) out;

vec4 corners[6];
vec3 u, v;

void EmitCorner( int i )
{
	gl_Position = corners[i];
	EmitVertex();
}

 void main()
{
	vec3 r = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;

	float divW = gl_in[0].gl_Position.w;

	float wdt = length( r )*0.05;
	vec3 d = normalize(r);
	if( dot(d, vec3(0,1,0) ) > 0.95 )
	{
		u = vec3(1,0,0)*wdt;
		v = vec3(0,0,1)*wdt;
	}
	else
	{
		u = vec3(0,1,0)*wdt;
		v = vec3( cross(u,d) );
	}

	// bottom verts

	vec4 start = VP*gl_in[0].gl_Position;
	vec4 end = VP*gl_in[1].gl_Position;

	// Omits very small branches (in screen size)
	if( length( start.xy - end.xy )/start.w < 0.01 )
		return;

	u = mat3(VP)*u;
	v = mat3(VP)*v;
	corners[0] = start + vec4(u,0);
	corners[1] = start - vec4(u,0);
	corners[2] = start + vec4(v,0);
										   
	// top verts						   
	corners[3] = end + vec4(u,0);
	corners[4] = end - vec4(u,0);
	corners[5] = end + vec4(v,0);


	EmitCorner(0);
	EmitCorner(3);
	EmitCorner(1);
	EmitCorner(4);
	EmitCorner(2);
	EmitCorner(5);
	EmitCorner(0);
	EmitCorner(3);
}

