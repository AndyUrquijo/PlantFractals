#version 110

#define FOG_DIST 1000

in vec3 go_normal;
in vec3 go_color;

const float amb_val = 0.3f;
const float dif_val = 0.7f;

const vec3 dirLight = normalize(vec3(1,1,0));
const vec3 fogColor = vec3(0.7,0.7,0.7);

uniform sampler2D colorMap;

void main()
{


	float ambient = amb_val;


	float diffuse = dot( normalize(go_normal), dirLight);
	//diffuse = (diffuse + 1.0)/1.5;
	diffuse = clamp(diffuse,0.0,1.0)*dif_val;

	float factor = (ambient + diffuse);
	//factor = floor(factor*3)/3;

	gl_FragColor.rgb = go_color;
	//gl_FragColor = texture2D( colorMap, vec2(0.5,0));
	gl_FragColor.rgb *= factor;


	float depth = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = depth/FOG_DIST;
	fogFactor = clamp(fogFactor,0,1);
	gl_FragColor.rgb = mix(gl_FragColor.rgb, fogColor, fogFactor);



	gl_FragColor.a = 1;
}
