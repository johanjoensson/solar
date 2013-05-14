#include "ship.h"
#include <iostream>

void Ship::draw(int program)
{
    glUniform1i(glGetUniformLocation(program, "spaceship"), 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    glUniform1i(glGetUniformLocation(program, "spaceship"), 0);
}

Ship::Ship(const char* model, const char *texture) : Object(model, texture)
{
    //static_rotation = Rz(M_PI) * Ry(3.14/2);
    static_rotation = Rz(0) * Ry(M_PI) * Rx(-0.5);
    rotate('y', 3.14/2);
    translate(0,-1.0,-2.2);
    //set_scale(1/30.0);

}

void Ship::set_rotation(vec3 rotation) {
    rot_mat = static_rotation * Rx(rotation.x) * Ry(rotation.y) * Rz(rotation.z);
    update();
}

void Ship::handle_movement(int xrel, int yrel, int width, int height)
{
    float d_fi = ((float)xrel)/width*8*M_PI;
    float d_theta = ((float)yrel)/height*4*M_PI;

    set_rotation(vec3(d_theta,0,d_fi));
    //set_rotation(vec3(-d_fi,0,-d_theta));
}
