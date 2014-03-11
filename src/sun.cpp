#include "sun.h"
#include <iostream>
using namespace std;

Sun::Sun(Model* model, const char* texture) : Body(model, texture)
{
    mass = 1;
    spin_x = 0;
    spin_y = 0;
    spin_z = 0;
    set_radius(1);
    velocity = vec3(0.0, 0.0, 0.0);
    vec3 zero = vec3(0.0, 0.0, 0.0);
#ifdef GRAV_OPT
    kv1 = zero;
    kv2 = zero;
    kv3 = zero;
    kv4 = zero;
    
    kr1 = zero;
    kr2 = zero;
    kr3 = zero;
    kr4 = zero;
    specularExponent = 2;
#endif //GRAV_OPT
}

void Sun::draw(int program)
{
    glUniform1i(glGetUniformLocation(program, "sun"), 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    sun_position = this->position;
    glUniform3f(glGetUniformLocation(program, "sun_position"), sun_position.x, sun_position.y, sun_position.z);
    glUniform3f(glGetUniformLocation(program, "emit_color"), emit_color.x, emit_color.y, emit_color.z);
    //FIXME ändra så att det är specularExponent som skickar upp och ej 10.0
    //direkt
    glUniform1f(glGetUniformLocation(program, "specularExponent"), (const GLfloat) specularExponent);
    glUniform1i(glGetUniformLocation(program, "sun"), 0);
}
