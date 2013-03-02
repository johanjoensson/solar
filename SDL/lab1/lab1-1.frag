#version 150

in vec3 ex_colour;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ex_colour, 1.0);
}
