//Leaf.gp
#version 110

//#define LINE_TEST

uniform mat4 VP;

layout(triangles) in;

#ifdef LINE_TEST
layout (line_strip, max_vertices=8) out;
#else
layout (triangle_strip, max_vertices=8) out;
#endif

in float vo_level[];
in float vo_delay[];
in vec3 vo_normal[];
out vec3 go_normal;
out vec3 go_color;

const vec3 colorLeaf = vec3(0.40, 0.80, 0.10); 
const vec3 colorBranch = vec3(0.00, 0.00, 1.00); 


//const vec3 colorB = vec3(0.10, 0.70, 0.00); // end

#define PI 3.1415926535897932384626433832795

vec3 Rotate( vec3 v, float a, vec3 n )
{
	return v*cos(a) + ( cross(v,n) )*sin(a) + n*dot(v,n)*(1 - cos(a));		// Rodrigues rotation formula
}



void EmitCorner( int i )
{
	EmitVertex();
}

bool IsOutOfView( vec2 v )
{
	return v.x > 1 || v.x < -1  || v.y > 1 || v.y <-1 ;
}

void main()
{

	vec4 Bs = vec4(gl_in[0].gl_Position.xyz,1);		// branch start
	vec4 Be = vec4(gl_in[1].gl_Position.xyz,1);		// branch end

	vec4 Rs = Bs + (Be - Bs)*vo_level[2]; // leaf start
	//vec4 Rs = Bs;
	vec4 Re = Rs +  vec4(gl_in[2].gl_Position.xyz,0); // leaf end

	vec3 N = vo_normal[2]; //end normal



	
#ifdef LINE_TEST
	//go_color = colorBranch;
	//gl_Position = VP*Bs;
	//EmitVertex();
	//gl_Position = VP*Be;
	//EmitVertex();
	//EndPrimitive();

	go_color = colorLeaf;
	gl_Position = VP*Rs;
	EmitVertex();
	gl_Position = VP*Re;
	EmitVertex();
	EndPrimitive();

	go_color = colorBranch;
	gl_Position = VP*Rs;
	EmitVertex();
	gl_Position = VP*(Rs+vec4(N,0));
	EmitVertex();
	EndPrimitive();

#else


	vec3 r = (Re - Rs).xyz;

	float wdt = 1.0;
	vec4 Rr = vec4(mix(Rs, Re, 0.1).xyz, 1);
	vec4 Rm = vec4(mix(Rr, Re, 0.3).xyz, 1);
	vec4 dRp = vec4(N*1.0, 0);

	vec3 ru = normalize(r);

	// Omits very small branches (in screen size)
	//if( length( Rs.xy - Re.xy )/Rs.w < 0.01 )
	//	return;
	//
	//if( IsOutOfView( Rs.xy/Rs.w ) && IsOutOfView( Re.xy/Re.w ) )
	//	return;

	//corners[0] = VP * Rs;
	//corners[1] = VP * Re;
	//
	//corners[2] = VP * (Rm + dRp);
	//corners[3] = VP * (Rm - dRp);


	go_color = colorLeaf;
	go_normal = N;

	//Leaf
	gl_Position = VP * (Rm + dRp);
	EmitVertex();
	gl_Position = VP * Rr;
	EmitVertex();
	gl_Position = VP * Re;
	EmitVertex();
	gl_Position = VP * (Rm - dRp);
	EmitVertex();
	EndPrimitive();

	Rr = vec4(mix(Rs, Re, 0.15).xyz, 1);

	//Stem
	gl_Position = VP * Rs;
	EmitVertex();
	gl_Position = VP * (Rr + 0.1*dRp);
	EmitVertex();
	gl_Position = VP * (Rr - 0.1*dRp);
	EmitVertex();
#endif


}

