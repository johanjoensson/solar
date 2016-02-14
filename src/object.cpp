#include "object.h"

#include <SDL2/SDL_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Object::Object(const char *model) : m(LoadModelPlus((char*)model)) {}

Object::Object(Model *model, const char *tex) : surface(IMG_Load(tex)), m(model)
{
    // Typical Texture Generation Using Data From The Bitmap
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

Object::Object(const char *model, const char *tex) : Object(LoadModelPlus((char*)model), tex) {}

void Object::set_scale(float s)
{
    scale = s;
    scale_mat = glm::scale(glm::mat4(), glm::vec3(scale, scale, scale));
    update();
}

float Object::get_scale()
{
    return scale;
}

void Object::draw(int program)
{
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    DrawModel(m, program, "in_position", "in_normal", NULL);
}

void Object::rotate(char direction, float angle)
{
    switch (direction) {
        case 'x':
            // Rotate around x
            rot_mat = glm::rotate(rot_mat, angle, glm::vec3(1, 0, 0));
            update();
            break;
        case 'y':
            // Rotate around y
            rot_mat = glm::rotate(rot_mat, angle, glm::vec3(0, 1, 0));
            update();
            break;
        case 'z':
            // Rotate around z
            rot_mat = glm::rotate(rot_mat, angle, glm::vec3(0, 0, 1));
            update();
            break;
    }
}

void Object::place(glm::vec3 pos)
{
    trans_mat = glm::translate(glm::mat4(), pos);
    update();
}

void Object::translate(float dx, float dy, float dz)
{
    trans_mat = glm::translate(trans_mat, glm::vec3(dx, dy, dz));
    update();
}

void Object::update()
{
    matrix = trans_mat * rot_mat * scale_mat;
}
