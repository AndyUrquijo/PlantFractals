#version 330

//varying float fogVal;
const vec4 fogColor = vec4(0.5,0.5,0.5,1.0);
const vec4 plantColor = vec4(0,0.7,0,1);
void main()
{
	gl_FragColor = plantColor;//mix(plantColor,fogColor, fogVal);
}
