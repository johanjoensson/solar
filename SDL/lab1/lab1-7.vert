#version 150

uniform mat4 in_Matrix1;
uniform mat4 in_Matrix2;
uniform mat4 in_Matrix3;

in  vec3 in_Position;
in vec3 in_Normal;


out vec3 ex_Normal;

void main(void)
{
	mat4 myMatrix = in_Matrix3*in_Matrix2*in_Matrix1;
	mat3 normMatrix = mat3(myMatrix);

	vec3 transNormal = normMatrix * in_Normal;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
	ex_Normal = transNormal;
}
