#include "spacebox.h"
#include <GL/gl.h>

#include <glm/gtc/type_ptr.hpp>

Spacebox::Spacebox(const char* model, const char* tex) : Object(model, tex)
{}

Spacebox::Spacebox()
{}

void Spacebox::draw(int program)
{
    glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1i(glGetUniformLocation(program, "spacebox"), 1);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glBindTexture(GL_TEXTURE_2D, texture);
    DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    glUniform1i(glGetUniformLocation(program, "spacebox"), 0);
    glEnable(GL_DEPTH_TEST);
}
