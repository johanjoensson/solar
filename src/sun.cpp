#include "sun.h"
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Sun::Sun(Model* model, const char* texture, GLuint shader, GLuint planet_shader) : Body(model, texture, shader),
    planet_shader(planet_shader)
{
    specularExponent = 2;
}

void Sun::draw()
{
    glUseProgram(planet_shader);
    glUniform3f(glGetUniformLocation(planet_shader, "sun_position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(planet_shader, "emit_color"), emit_color.x, emit_color.y, emit_color.z);
    glUniform1f(glGetUniformLocation(planet_shader, "specularExponent"), (const GLfloat) specularExponent);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, shader, "in_position", nullptr, "in_tex_coord");
}
