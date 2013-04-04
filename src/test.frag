#version 150

in vec3 out_normal;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(abs(out_normal.x), abs(out_normal.y), abs(out_normal.z),0);
}
