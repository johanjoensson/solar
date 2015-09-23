#include "ship.h"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

void Ship::draw(int program)
{
    glUniform1i(glGetUniformLocation(program, "spaceship"), 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    glUniform1i(glGetUniformLocation(program, "spaceship"), 0);
}

Ship::Ship(const char* model, const char *texture) : Object(model, texture)
{
    //static_rotation = Rz(M_PI) * Ry(3.14/2);
    static_rotation = glm::rotate(glm::mat4(), float(-0.5), glm::vec3(1,0,0));
    static_rotation = glm::rotate(static_rotation, float(M_PI), glm::vec3(0,1,0));
    static_rotation = glm::rotate(static_rotation, float(0), glm::vec3(0,0,1));
    rotate('y', 3.14/2);
    translate(0,-0.5,-1.2);
    //set_scale(1/30.0);
    set_scale(1/2.0);

}

void Ship::set_rotation(vec3 rotation) {
    // TODO(gustav) är detta rätt?
    rot_mat = glm::rotate(static_rotation, glm::l2Norm(rotation), rotation);
    update();
}

void Ship::handle_movement(int xrel, int yrel, int width, int height)
{
    float d_fi = ((float)xrel)/width*8*M_PI;
    float d_theta = ((float)yrel)/height*4*M_PI;

    set_rotation(vec3(d_theta,0,d_fi));
}
