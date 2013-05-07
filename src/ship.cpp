#include "ship.h"

void Ship::draw(int program)
{
    glUniform1i(glGetUniformLocation(program, "spaceship"), 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    glUniform1i(glGetUniformLocation(program, "spaceship"), 0);
}
