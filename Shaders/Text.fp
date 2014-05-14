#version 110

uniform sampler2D colorMap;

varying vec2 var_texCoord;

void main()
{
	vec4 color = texture2D(colorMap, var_texCoord,0);
	gl_FragColor = vec4(1,1,1,color.r);
}
