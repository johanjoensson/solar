#version 150

in vec3 ex_Normal;

out vec4 out_Color;

void main(void)
{
	const vec3 light = vec3(0.58f, 0.58f, 0.58f);
	float shade;
	shade = dot(normalize(ex_Normal), light);
	vec3 colour = vec3(max(ex_Normal[2], max(ex_Normal[0], ex_Normal[1])));
	colour = clamp(colour, 0, 1);
	out_Color = vec4(shade*colour, 1.0);
}
