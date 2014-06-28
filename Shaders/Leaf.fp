#version 110

in vec3 go_normal;
in vec3 go_color;

const vec3 dirLight = normalize(vec3(1,1,0));

void main()
{
	float ambient = 0.3;


	float diffuse = dot( normalize(go_normal), dirLight);
	diffuse = clamp(diffuse,0.0,1.0)*0.7;

	gl_FragColor.rgb = go_color*(ambient + diffuse);

	gl_FragColor.a = 1;
}
