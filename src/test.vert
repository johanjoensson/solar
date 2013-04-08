#version 150

in vec3 in_position;
in vec3 in_normal;
in vec2 in_tex_coord;

out vec3 out_normal;
out vec2 out_tex_coord;

uniform mat4 proj_matrix;
uniform mat4 mdl_matrix;
uniform mat4 cam_matrix;
uniform int spacebox;

void main(void)
{
        out_normal = in_normal;
        out_tex_coord = in_tex_coord;
        if(spacebox == 0){
                gl_Position = proj_matrix * cam_matrix * mdl_matrix * vec4(in_position, 1.0);
        } else {
                gl_Position = proj_matrix * mat4(mat3(cam_matrix)) * mdl_matrix * vec4(in_position, 1.0);
        }
}
