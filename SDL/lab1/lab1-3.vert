#version 150

uniform mat4 in_Matrix;

in  vec3 in_Position;
in vec3 in_Colour;

out vec3 ex_Colour;

void main(void)
{
	gl_Position = in_Matrix*vec4(in_Position, 1.0);
	ex_Colour = in_Colour;
}
