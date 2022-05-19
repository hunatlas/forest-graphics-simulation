#version 330 core

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir;

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.3, 0.6, 0.7);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);
uniform vec3 Ls = vec3(0.9, 0.9, 0.9);

uniform vec3 eyePos;

uniform sampler2D texImage;

void main()
{


	vec3 ambient = La;

	vec3 l = normalize(light_dir);
	vec3 n = normalize(vs_out_norm);
	float di = clamp(dot(-l, n), 0, 1);
	vec3 diffuse = di * Ld;

	vec3 r = reflect(l, n);
	vec3 toEye = normalize(eyePos - vs_out_pos);
	float si = pow(clamp(dot(r, toEye), 0, 1), 16);
	vec3 specular = si * Ls;
	
	fs_out_col = vec4(ambient + diffuse + specular, 1) * texture(texImage, vs_out_tex);
}