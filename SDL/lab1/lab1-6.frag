#version 150

in vec3 ex_Colour;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ex_Colour, 1.0);
}
