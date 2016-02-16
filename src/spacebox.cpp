#include "spacebox.h"
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>

Spacebox::Spacebox(const char* model, const char* tex, GLuint shader) : Object(model, tex, shader)
{}

void Spacebox::draw()
{
    glUseProgram(shader);
    glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, shader, "in_position", nullptr, "in_tex_coord");
    glEnable(GL_DEPTH_TEST);
}
