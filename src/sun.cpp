#include "sun.h"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Sun::Sun(Model* model, const char* texture, GLuint shader) : Body(model, texture, shader)
{
    specularExponent = 2;
}

void Sun::draw()
{
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "sun"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, shader, "in_position", "in_normal", "in_tex_coord");
    sun_position = this->position;
    glUniform3f(glGetUniformLocation(shader, "sun_position"), sun_position.x, sun_position.y, sun_position.z);
    glUniform3f(glGetUniformLocation(shader, "emit_color"), emit_color.x, emit_color.y, emit_color.z);
    glUniform1f(glGetUniformLocation(shader, "specularExponent"), (const GLfloat) specularExponent);
    glUniform1i(glGetUniformLocation(shader, "sun"), 0);
}
