#version 150

in vec2 out_tex_coord;
in vec3 out_normal;
in vec3 out_position; 
in vec3 out_position_cam;

out vec4 out_Color;

uniform sampler2D texUnit;
uniform int spacebox;
uniform mat4 cam_matrix;

uniform vec3 sun_position;
uniform vec3 emit_color;
uniform float specularExponent;
uniform int sun;
uniform mat4 mdl_matrix;

mat3 light_cam_matrix = mat3(cam_matrix);

vec3 s;
vec3 n;
vec3 eye;
vec3 tmp_colors;
vec3 colors;

vec3 diffuse;
vec3 specular;
vec3 reflection;

float cos_angle;
uniform int spaceship;

void main(void)
{
        if(sun == 1 || spacebox == 1 || spaceship == 1) {
                out_Color = texture(texUnit, out_tex_coord);
        } else if(spacebox == 0){
                tmp_colors = vec3(texture(texUnit, out_tex_coord));
                colors = vec3(0.1,0.1,0.00)*tmp_colors;
                s = normalize(light_cam_matrix * sun_position - out_position);
                n = normalize(out_normal);

                float lambert = dot(n, s);

                if(lambert > 0){
                        diffuse = (emit_color*tmp_colors)*lambert;
                        colors += diffuse;

                        reflection = reflect(s, n);
                        eye = normalize(out_position_cam);

                        cos_angle = dot(reflection, eye);
                        cos_angle = max(0, cos_angle);

                        specular = (emit_color*tmp_colors)*pow(cos_angle, specularExponent);
                        colors += specular;
                }
                out_Color = vec4(colors, 1);
        } else {
                out_Color = texture(texUnit, out_tex_coord);
        }
}
