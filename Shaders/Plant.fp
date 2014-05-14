#version 110

//varying float fogVal;
const vec4 plantColor = vec4(0.6,0.7,0.2,1);

varying vec3 var_color;

void main()
{
//	gl_FragColor = vec4(var_color,1);
	gl_FragColor = plantColor;
}
