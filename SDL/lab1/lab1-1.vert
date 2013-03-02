#version 150

in  vec3 in_Position;
in vec3 in_colour;

out vec3 ex_colour;

void main(void)
{
	gl_Position = vec4(in_Position, 1.0);
	ex_colour = in_colour;
}
