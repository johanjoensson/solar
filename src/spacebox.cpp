#include "spacebox.h"
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>

Spacebox::Spacebox(const char* model, const char* tex) : Object(model, tex)
{}

Spacebox::Spacebox()
{}

void Spacebox::draw(int program)
{
    glUseProgram(program);
    glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", nullptr, "in_tex_coord");
    glEnable(GL_DEPTH_TEST);
}
