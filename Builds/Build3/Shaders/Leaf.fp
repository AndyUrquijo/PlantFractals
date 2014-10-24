#version 110

#define FOG_DIST 1000

in vec3 go_normal;
in vec3 go_color;


const vec3 dirLight = normalize(vec3(1,1,0));
const vec3 fogColor = vec3(0.7,0.7,0.7);

void main()
{



	float ambient = 0.3;

	float diffuse = dot( normalize(go_normal), dirLight);
	diffuse = clamp(diffuse,0.0,1.0)*0.7;

	gl_FragColor.rgb = go_color*(ambient + diffuse);

	float depth = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = depth/FOG_DIST;
	fogFactor = clamp(fogFactor,0,1);
	gl_FragColor.rgb = mix(gl_FragColor.rgb, fogColor, fogFactor);

	gl_FragColor.a = 1;
}
