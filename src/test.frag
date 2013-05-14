#version 150

in vec2 out_tex_coord;
in vec3 out_normal;
in vec3 out_position; 
in vec3 out_surface_camera;

out vec4 out_Color;

uniform sampler2D texUnit;
uniform int spacebox;

uniform float sun_position[3];
uniform float emit_color[3];
uniform float specularExponent;



void main(void)
{
        if(spacebox == 0){

		vec4 diffuse = vec4(0.0);
		vec4 specular = vec4(0.0);
		float shade;
		float spec;
		vec3 direction;
		vec3 r;	
		vec3 da_color;

		  /* Vector from surface to light source */
		direction.x = out_position.x - sun_position[0];
		direction.y = out_position.y - sun_position[1];
		direction.z = out_position.z - sun_position[2]; 
		  /* Diffuse shading */
		shade = dot(normalize(out_normal), normalize(-direction));
		shade = clamp(shade, 0, 1);

		/* Specular shading */
		r = reflect(direction, out_normal);
		/* Add the specular exponent to the cosine function */
		spec = pow(dot(normalize(r), normalize(out_surface_camera)), specularExponent);
		spec = clamp(spec, 0, 1);       
 		

		out_Color = texture(texUnit, out_tex_coord) * length(out_normal);
		
		da_color.x = emit_color[0]; 
		da_color.y = emit_color[1];
		da_color.z = emit_color[2];		

		diffuse = out_Color*vec4(shade*da_color, 1.0);
		specular = out_Color*vec4(spec*da_color, 1.0);
        	out_Color = diffuse; // + specular;

        } else {

                out_Color = texture(texUnit, out_tex_coord);

        }
}
