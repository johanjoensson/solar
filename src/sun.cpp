#include "sun.h"
#include <iostream>
using namespace std;

Sun::Sun(const char* model, const char* texture) : Body(model, texture)
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
#endif //GRAV_OPT

}

void Sun::draw(int program)
{
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    glBindTexture(GL_TEXTURE_2D, texture);
    cout << "det är inte fel på rad två i sun.draw" << endl;
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    cout << "om jag kommer heller ej på rad tre/fyra" << cout;
    emit_color[0] = 1.0;
    emit_color[1] = 1.0;
    emit_color[2] = 1.0;
    sun_position[0] = (GLfloat) (this->position.x);
    sun_position[1] = (GLfloat) (this->position.y);
    sun_position[2] = (GLfloat) (this->position.z);
    glUniform3fv(glGetUniformLocation(program, "sun_position"), 3, (const GLfloat*) (sun_position));
    glUniform3fv(glGetUniformLocation(program, "emit_color"), 3, emit_color);
    cout << "uploadz emit color from sun::draw" << endl;
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);
    
}
