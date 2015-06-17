//Leaf.gp
#version 110

//#define LINE_TEST

uniform mat4 VP;
uniform vec3 CAMERA_POS;

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

const vec3 colorLeafA = vec3(0.1, 0.70, 0.05); 
const vec3 colorLeafB = vec3(0.2, 0.40, 0.0); 
const vec3 colorBranch = vec3(0.00, 0.00, 1.00); 

#define PI 3.1415926535897932384626433832795

vec3 Rotate( vec3 v, float a, vec3 n )
{
	return v*cos(a) + ( cross(v,n) )*sin(a) + n*dot(v,n)*(1 - cos(a));		// Rodrigues rotation formula
}




bool IsOutOfView( vec2 v )
{
	return v.x > 1 || v.x < -1  || v.y > 1 || v.y <-1 ;
}

void main()
{

	vec4 Bs = vec4(gl_in[0].gl_Position.xyz,1);		// branch start
	vec4 Be = vec4(gl_in[1].gl_Position.xyz,1);		// branch end

	vec4 Rs = Bs + (Be - Bs)*vo_level[2]*(-1); // leaf start
	//vec4 Rs = Bs;
	vec3 L = normalize(gl_in[2].gl_Position.xyz); // leaf stem vector
	vec4 Re = Rs + vec4(L,0); // leaf end

	vec3 N = vo_normal[2]; //end normal
	vec3 T = normalize(cross(L,N));

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
	gl_Position = VP*(Rs+vec4(T,0));
	EmitVertex();
	EndPrimitive();

#else


	vec3 r = (Re - Rs).xyz;

	float wdt = 1.0;
	vec4 Rr = vec4(mix(Rs, Re, 0.1).xyz, 1);
	vec4 Rm = vec4(mix(Rr, Re, 0.3).xyz, 1);
	vec4 dRp = vec4(T*0.7, 0);

	vec3 ru = normalize(r);

	// Omits very small branches (in screen size)
	//if( length( Rs.xy - Re.xy )/Rs.w < 0.01 )
	//	return;
	//
	//if( IsOutOfView( Rs.xy/Rs.w ) && IsOutOfView( Re.xy/Re.w ) )
	//	return;

	vec4 corners[4];
	corners[0] = VP * Rr; 
	corners[1] = VP * Re; 
	corners[2] = VP * (Rm + dRp); 
	corners[3] = VP * (Rm - dRp); 
	
	vec3 toCam = Rm - (-CAMERA_POS); // HAX: Camera seems to be... backwards?

	float N_sign = (dot(N, toCam) > 0 ? 0: 1)*2 - 1;
	go_normal = N*N_sign;
	
	//Leaf
	go_color = colorLeafB;
	gl_Position = corners[3];
	EmitVertex();

	go_color = colorLeafA;
	gl_Position = corners[0];
	EmitVertex();
	gl_Position = corners[1];
	EmitVertex();

	go_color = colorLeafB;
	gl_Position = corners[2];
	EmitVertex();

#endif


}

