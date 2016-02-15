#version 150

in vec3 in_position;
in vec2 in_tex_coord;

out vec2 out_tex_coord;

uniform mat4 proj_matrix;
uniform mat4 mdl_matrix;
uniform mat4 cam_matrix;

void main(void)
{
        out_tex_coord = in_tex_coord;
        gl_Position = proj_matrix * mat4(mat3(cam_matrix)) * mdl_matrix * vec4(in_position, 1.0);
}
