#version 150

in vec3 in_position;
in vec3 in_normal;
in vec2 in_tex_coord;

out vec3 out_normal;
out vec2 out_tex_coord;
out vec3 out_position;
out vec3 out_position_cam;

uniform mat4 proj_matrix;
uniform mat4 mdl_matrix;
uniform mat4 cam_matrix;
uniform vec3 camera_pos;

void main(void)
{
        out_normal = mat3(cam_matrix * mdl_matrix) * in_normal;
        out_tex_coord = in_tex_coord;
        out_position = mat3(cam_matrix) * vec3(mdl_matrix * vec4(in_position, 1));
        out_position_cam = vec3(cam_matrix * mdl_matrix * vec4(in_position, 1));
        gl_Position = proj_matrix * cam_matrix * mdl_matrix * vec4(in_position, 1.0);
}
