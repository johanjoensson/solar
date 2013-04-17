#include "object.h"
#include "GL_utilities.h"
#include "soil/src/SOIL.h"

Object::Object(){
    reflectivity = 1;
    scale = 1;
    matrix = IdentityMatrix();
    scale_mat = S(scale, scale, scale);
    rot_mat = IdentityMatrix();
    trans_mat = IdentityMatrix();
}

Object::Object(const char *model)
{
    m = LoadModelPlus((char*)model);
    reflectivity = 1;
    scale = 1;
    matrix = IdentityMatrix();
    scale_mat = S(scale, scale, scale);
    rot_mat = IdentityMatrix();
    trans_mat = IdentityMatrix();
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
    matrix = IdentityMatrix();
    scale_mat = S(scale, scale, scale);
    rot_mat = IdentityMatrix();
    trans_mat = IdentityMatrix();
}

void Object::draw(int program){
    glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_TRUE, matrix.m);
    DrawModel(m, program, "in_position", "in_normal", NULL);
}

void Object::rotate(char direction, float angle)
{
    switch (direction) {
        case 'x':
            // Rotate around x
            rot_mat = Rx(angle) * rot_mat;
            this->update();
            break;
        case 'y':
            // Rotate around y
            rot_mat = Ry(angle) * rot_mat;
            this->update();
            break;
        case 'z':
            // Rotate around z
            rot_mat = Rz(angle) * rot_mat;
            this->update();
            break;
    }
}

void Object::place(vec3 pos)
{
    trans_mat = T(pos.x, pos.y, pos.z);
    this->update();
}

void Object::translate(float dx, float dy, float dz)
{
    trans_mat = T(dx, dy, dz)* trans_mat;
    this->update();
}

void Object::update()
{
    //matrix = trans_mat * rot_mat * scale_mat;
    matrix = trans_mat * rot_mat;
}
