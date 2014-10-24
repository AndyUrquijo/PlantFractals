#version 330

uniform sampler2D colorMap;

varying vec4 var_color;

void main()
{
	gl_FragColor = var_color;
}
