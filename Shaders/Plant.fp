#version 110

//varying float fogVal;
const vec3 plantColor = vec3(0.6,0.7,0.2);

in vec3 go_normal;
in vec3 go_color;

const vec3 dirLight = normalize(vec3(1,1,0));

void main()
{
//	gl_FragColor = vec4(var_color,1);
	float diffuse = dot( normalize(go_normal), dirLight);
	diffuse = clamp(diffuse,0.0,1.0)*0.7;
	float ambient = 0.3;
	gl_FragColor.rgb = go_color*(ambient + diffuse);
	//gl_FragColor.rgb = plantColor;
	gl_FragColor.a = 1;
}
