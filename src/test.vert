#version 150

in vec3 in_Position;

uniform mat4 proj_matrix;
uniform mat4 mdl_matrix;

void main(void)
{
	gl_Position = proj_matrix * mdl_matrix * vec4(in_Position, 1.0);
}
