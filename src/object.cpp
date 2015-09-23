#include "object.h"
#include "GL_utilities.h"
#include "soil/src/SOIL.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Object::Object(){
    reflectivity = 1;
    scale = 1;
    matrix = glm::mat4();
    scale_mat = glm::mat4(scale);
    rot_mat = glm::mat4();
    trans_mat = glm::mat4();
}

Object::Object(const char *model)
{
    m = LoadModelPlus((char*)model);
    reflectivity = 1;
    scale = 1;
    matrix = glm::mat4();
    scale_mat = glm::mat4(scale);
    rot_mat = glm::mat4();
    trans_mat = glm::mat4();
}

Object::Object(Model *model, const char *tex)
{
    m = model; 

    texture = SOIL_load_OGL_texture(
            tex,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y
            );

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //LoadTGATextureSimple(tex, &texture);
    reflectivity = 1;
    scale = 1;
    matrix = glm::mat4();
    scale_mat = glm::mat4(scale);
    rot_mat = glm::mat4();
    trans_mat = glm::mat4();
}

Object::Object(const char *model, const char *tex)
{
    m = LoadModelPlus((char*)model); 

    texture = SOIL_load_OGL_texture(
            tex,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_INVERT_Y
            );

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //LoadTGATextureSimple(tex, &texture);
    reflectivity = 1;
    scale = 1;
    matrix = glm::mat4();
    scale_mat = glm::mat4(scale);
    rot_mat = glm::mat4();
    trans_mat = glm::mat4();
}

void Object::set_scale(float s)
{
    scale = s;
    scale_mat = glm::mat4(scale);
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
