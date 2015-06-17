//Plant.gp
#version 430

//#define LINE_TEST

uniform mat4 VP;

layout(lines) in;

#ifdef LINE_TEST
layout (line_strip, max_vertices=8) out;
#else
layout (triangle_strip, max_vertices=8) out;
#endif

in float vo_level[];
in vec3 vo_normal[];
in float vo_delay[];

out vec3 go_normal;
out vec3 go_color;

vec4 corners[6];


const vec3 colorA = vec3(0.20, 0.15, 0.00); // start
const vec3 colorB = vec3(0.15, 0.30, 0.00); // end


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

vec3 dNs[3];
vec3 dNe[3];


	vec4 Rs = vec4(gl_in[0].gl_Position.xyz,1); //start position
	vec4 Re = vec4(gl_in[1].gl_Position.xyz,1); //end position
	
	vec4 Ns = vec4(vo_normal[0],0); //start normal
	vec4 Ne = vec4(vo_normal[1],0); //end normal


	vec3 r = (Re - Rs).xyz;

	float wdt[2];
	//wdt[0] = 5.0*exp(-2.5*vo_level[0]);
	//wdt[1] = 5.0*exp(-2.5*vo_level[1]);
	
	//wdt[0] = length( r )*0.06;
	//wdt[1] = length( r )*0.04;

	float maxLevel = 8;
	wdt[0] = (maxLevel - vo_level[0])*0.1;
	wdt[1] = (maxLevel - vo_level[1])*0.1;



	vec3 ru = normalize(r);
	
	dNs[0] = normalize(Ns.xyz);
	dNs[1] = Rotate(dNs[0], PI*2/3, ru);
	dNs[2] = Rotate(dNs[0], -PI*2/3, ru);

	dNe[0] = normalize(Ne.xyz);
	dNe[1] = Rotate(dNe[0], PI*2/3, ru);
	dNe[2] = Rotate(dNe[0], -PI*2/3, ru);




	corners[0] = VP * (Rs + vec4(dNs[0]*wdt[0],0) );
	corners[1] = VP * (Rs + vec4(dNs[1]*wdt[0],0) );
	corners[2] = VP * (Rs + vec4(dNs[2]*wdt[0],0) );

	corners[3] = VP * (Re + vec4(dNe[0]*wdt[1],0) );
	corners[4] = VP * (Re + vec4(dNe[1]*wdt[1],0) );
	corners[5] = VP * (Re + vec4(dNe[2]*wdt[1],0) );
	
	// Omits very small branches (in screen size)
	//if( length( corners[0].xy - corners[3].xy )/corners[0].w < 0.01 )
	//	return;
	//
	//if( IsOutOfView( corners[0].xy/corners[0].w ) && IsOutOfView( corners[3].xy/corners[3].w ) )
	//	return;


	vec3 colorEnd = mix( colorA, colorB, vo_level[1] /maxLevel);
	vec3 colorStart = mix( colorA, colorB, vo_level[0] /maxLevel);

#ifdef LINE_TEST
	go_color = colorStart;
	gl_Position = VP *Rs;
	EmitVertex();
	go_color = colorEnd;
	gl_Position = VP *Re;
	EmitVertex();
	EndPrimitive();

	go_color = vec3(0.5,0.5,0);
	gl_Position = VP *Re;
	EmitVertex();
	gl_Position = VP *(Re+Ne);
	EmitVertex();

	return;
#endif


	go_normal = dNs[0];
	go_color = colorStart;
	EmitCorner(0);
	go_normal = dNe[0];
	go_color  = colorEnd;
	EmitCorner(3);

	go_normal = dNs[1];
	go_color  = colorStart;
	EmitCorner(1);
	go_normal = dNe[1];
	go_color  = colorEnd;
	EmitCorner(4);
	
	go_normal = dNs[2];
	go_color  = colorStart;
	EmitCorner(2);
	go_normal = dNe[2];
	go_color  = colorEnd;
	EmitCorner(5);

	go_normal = dNs[0];
	go_color  = colorStart;
	EmitCorner(0);
	go_normal = dNe[0];
	go_color  = colorEnd;
	EmitCorner(3);
}

