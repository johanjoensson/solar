#version 150

in vec2 out_tex_coord;
in vec3 out_normal;

out vec4 out_Color;

uniform sampler2D texUnit;
uniform int spacebox;
uniform int spaceship;

void main(void)
{
        if(spacebox == 0 && spaceship == 0){
                out_Color = texture(texUnit, out_tex_coord) * length(out_normal);
        } else if(spacebox == 0 && spaceship == 1) {
                out_Color = texture(texUnit, out_tex_coord) * length(out_normal);
                //out_Color = vec4(0,1,0,0); // Grön
        } else {
                out_Color = texture(texUnit, out_tex_coord);
        }
}
