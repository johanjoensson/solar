#include "planetoids.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "soil/src/SOIL.h"

Planetoids::Planetoids()
{
    nb = 0;
    program = 0;
}


Planetoids::Planetoids(int num, const char* model) : Body()
{
    m = LoadModelPlus((char*)model);
    nb = num;
    for(int i = 0; i < num; i++){
        reflectivity[i] = 1;
        scale[i] = 1;
        matrix[i] = IdentityMatrix();
        scale_mat[i] = S(scale[i],scale[i],scale[i]);
        rot_mat[i] = IdentityMatrix();
        trans_mat[i] = IdentityMatrix();

        position[i] = vec3(0,0,0);
        velocity[i] = vec3(0,0,0);
    }
}

Planetoids::Planetoids(int num, const char *model, const char *tex) : Body()
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
    for(int i = 0; i < num; i++){
        reflectivity[i] = 1;
        scale[i]  = 1;
        matrix[i]  = IdentityMatrix();
        scale_mat[i]  = S(scale[i], scale[i], scale[i]);
        rot_mat[i]  = IdentityMatrix();
        trans_mat[i]  = IdentityMatrix();

        position[i] = vec3(0,0,0);
        velocity[i] = vec3(0,0,0);

    }
}

Planetoids::Planetoids(int num, const char *model, const char *tex, const char* vertex_shader, const char* fragment_shader) : Body()
{
    m = LoadModelPlus((char*)model);
	program = loadShaders(vertex_shader, fragment_shader);

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
    for(int i = 0; i < num; i++){
        reflectivity[i] = 1;
        scale[i]  = 1;
        matrix[i]  = IdentityMatrix();
        scale_mat[i]  = S(scale[i], scale[i], scale[i]);
        rot_mat[i]  = IdentityMatrix();
        trans_mat[i]  = IdentityMatrix();

        position[i] = vec3(0,0,0);
        velocity[i] = vec3(0,0,0);

    }
}

void matrix_update(int index)
{
    matrix[i] = trans_mat[i]*rot_mat[i]*scale_mat[i];
}

void Planetoids::rotate(vec3 direction, float angle, int index)
{
    rot_mat[index] = ArbRotate(direction, angle) * rot_mat[index];
    matrix_update(index);
}

void Planetoids::place(vec3 pos, int index)
{
    trans_mat[index] = T(pos.x, pos.y, pos.z);
    matrix_update(index);
}

void Planetoids::translate(float dx, float dy, float dz, int index)
{
    trans_mat[index] = T(dx, dy, dz)* trans_mat[index];
    matrix_update(index);
}
