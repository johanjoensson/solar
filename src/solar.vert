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
uniform int spacebox;
uniform vec3 camera_pos;
uniform int spaceship;

void main(void)
{
        out_normal = mat3(cam_matrix * mdl_matrix) * in_normal;
        out_tex_coord = in_tex_coord;
        //out_position = vec3(mdl_matrix*vec4(in_position, 1.0));
        out_position = mat3(cam_matrix) * vec3(mdl_matrix * vec4(in_position, 1));
        out_position_cam = vec3(cam_matrix * mdl_matrix * vec4(in_position, 1));
        if(spacebox == 0 && spaceship == 0){
                gl_Position = proj_matrix * cam_matrix * mdl_matrix * vec4(in_position, 1.0);
        } else if(spaceship == 1 && spacebox == 0)
                gl_Position = proj_matrix * mdl_matrix * vec4(in_position, 1.0);
        else {
                gl_Position = proj_matrix * mat4(mat3(cam_matrix)) * mdl_matrix * vec4(in_position, 1.0);
        } 
}
