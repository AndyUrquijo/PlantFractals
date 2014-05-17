//Plant.vp
#version 110

uniform mat4 WORLD;
uniform mat4 VP;

layout(lines) in;
layout (triangle_strip, max_vertices=8) out;


in float vo_level[];
in float vo_delay[];
in vec3 vo_normal[];
out vec3 go_normal;
out vec3 go_color;

vec4 corners[6];


const vec3 colorA = vec3(0.20, 0.15, 0.00); // start
const vec3 colorB = vec3(0.10, 0.70, 0.00); // end

#define PI 3.1415926535897932384626433832795

vec3 Rotate( vec3 v, float a, vec3 n )
{
	return v*cos(a) + ( cross(v,n) )*sin(a) + n*dot(v,n)*(1 - cos(a));		// Rodrigues rotation formula
}



void EmitCorner( int i )
{
	gl_Position = corners[i];
	EmitVertex();
}

bool IsOutOfView( vec2 v )
{
	return v.x > 1 || v.x < -1  || v.y > 1 || v.y <-1 ;
}

void main()
{

vec3 dN[3];
vec4 dNp[3];


	vec4 Rs = gl_in[0].gl_Position; //start position
	vec4 Re = gl_in[1].gl_Position; //end position
	
	vec3 Ns = vo_normal[0]; //start normal
	vec3 Ne = vo_normal[1]; //end normal

	vec3 r = (Re - Rs).xyz;


	float wdt = length( r )*0.06;
	vec3 ru = normalize(r);
	vec3 n = normalize(Ne);
	
	dN[0] = n;
	dN[1] = Rotate(n, PI*2/3, ru);
	dN[2] = Rotate(n, -PI*2/3, ru);

	Rs = VP*Rs;
	Re = VP*Re;

	// Omits very small branches (in screen size)
	if( length( Rs.xy - Re.xy )/Rs.w < 0.01 )
		return;
	
	if( IsOutOfView( Rs.xy/Rs.w ) && IsOutOfView( Re.xy/Re.w ) )
		return;

	dNp[0] = vec4( mat3(VP)*dN[0]*wdt , 0 );
	dNp[1] = vec4( mat3(VP)*dN[1]*wdt , 0 );
	dNp[2] = vec4( mat3(VP)*dN[2]*wdt , 0 );

	corners[0] = Rs + dNp[0];
	corners[1] = Rs + dNp[1];
	corners[2] = Rs + dNp[2];

	// top verts
	corners[3] = Re + dNp[0];
	corners[4] = Re + dNp[1];
	corners[5] = Re + dNp[2];
	


	vec3 colorStart = mix( colorA, colorB, vo_level[0] /7);
	vec3 colorEnd = mix( colorA, colorB, vo_level[1] /7);

	go_normal = dN[0];
	go_color = colorStart;
	EmitCorner(0);
	go_color  = colorEnd;
	EmitCorner(3);

	go_normal = dN[1];
	go_color  = colorStart;
	EmitCorner(1);
	go_color  = colorEnd;
	EmitCorner(4);
	
	go_normal = dN[2];
	go_color  = colorStart;
	EmitCorner(2);
	go_color  = colorEnd;
	EmitCorner(5);

	go_normal = dN[0];
	go_color  = colorStart;
	EmitCorner(0);
	go_color  = colorEnd;
	EmitCorner(3);
}

