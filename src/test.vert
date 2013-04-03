#version 150

in vec3 in_position;
in vec3 in_normal;

out vec3 out_normal;

uniform mat4 proj_matrix;
uniform mat4 mdl_matrix;

void main(void)
{
	gl_Position = proj_matrix * mdl_matrix * vec4(in_position, 1.0);
        out_normal = in_normal;
}
