#version 150

in vec2 out_tex_coord;
in vec3 out_normal;

out vec4 out_Color;

uniform sampler2D texUnit;
uniform int spacebox;

void main(void)
{
        if(spacebox == 0){
                out_Color = texture(texUnit, out_tex_coord) * vec4(out_normal, 0);
        } else {
                out_Color = texture(texUnit, out_tex_coord);
        }
}
