#version 110

#define FOG_DIST 1000

//const vec3 color = vec3(0.35,0.24,0.14)*0.5;
const vec3 color = vec3(0.1,0.1,0.1);
//const vec3 fogColor = vec3(0.7,0.7,0.7);
const vec3 fogColor = vec3(0.1,0.1,0.1);

void main()
{
	gl_FragColor = vec4(color,1);

	float depth = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = depth/FOG_DIST;
	fogFactor = clamp(fogFactor,0,1);
	gl_FragColor.rgb = mix(gl_FragColor.rgb, fogColor, fogFactor);

}
