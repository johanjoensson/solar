#include "planetoids.h"
#include "loadobj.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <SDL2/SDL_image.h>

#include "helper/GLShader.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        matrix[i] = glm::mat4();
        scale_mat[i] = glm::mat4(scale[i]);
        rot_mat[i] = glm::mat4();
        trans_mat[i] = glm::mat4();

        position[i] = glm::vec3(0,0,0);
        velocity[i] = 0;
    }
}

Planetoids::Planetoids(int num, const char *model, const char *tex) : Body()
{
    m = LoadModelPlus((char*)model);

    surface = IMG_Load(tex);

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //LoadTGATextureSimple(tex, &texture);
    for(int i = 0; i < num; i++){
        reflectivity[i] = 1;
        scale[i]  = 1;
        matrix[i]  = glm::mat4();
        scale_mat[i]  = glm::mat4(scale[i]);
        rot_mat[i]  = glm::mat4();
        trans_mat[i]  = glm::mat4();

        position[i] = glm::vec3(0,0,0);
        velocity[i] = 0;

    }
}

Planetoids::Planetoids(int num, float dist, const char *model, const char *tex, const char* vertex_shader, const char* fragment_shader) : Body()
{
    nb = num;
    reflectivity = new float[num];
    scale = new float[num];

    matrix = new glm::mat4[num];
    scale_mat = new glm::mat4[num];
    rot_mat = new glm::mat4[num];
    trans_mat = new glm::mat4[num];

    position = new glm::vec3[num];
    velocity = new float[num];
    rot_axis = new glm::vec3[num];
    
    // Frö för slumpvärden
    srand(time(NULL));

    m = LoadModelPlus((char*)model);
    program = LoadShader(vertex_shader, fragment_shader);

    surface = IMG_Load(tex);

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //LoadTGATextureSimple(tex, &texture);

    glm::vec3 tmp;
    tmp = glm::vec3(((float) rand()/RAND_MAX * 2 -1)/2,((float) rand()/RAND_MAX * 2 -1)/2,((float) rand()/RAND_MAX * 2 -1)/2);
    for(int i = 0; i < num; i++){
        reflectivity[i] = 1;
        scale[i]  = (float) rand()/RAND_MAX * 0.8;
        matrix[i]  = glm::mat4();
        scale_mat[i]  = glm::mat4(scale[i]);
        rot_mat[i]  = glm::mat4();
        trans_mat[i]  = glm::mat4();

        rot_mat[i] = glm::rotate(float(rand()/RAND_MAX * 2*M_PI), tmp);
        position[i] = glm::vec3(rot_mat[i]*glm::vec4((glm::vec3(dist,dist,dist)+dist*glm::vec3(((float) rand()/RAND_MAX * 2 -1)/5,((float) rand()/RAND_MAX * 2 -1)/2,((float) rand()/RAND_MAX * 2 -1)/2)), 1.0));
        velocity[i] = (float) rand()/RAND_MAX * 5;

        rot_axis[i] = tmp;

        place(position[i], i);

    }
}

void Planetoids::matrix_update(int index)
{
    matrix[index] = trans_mat[index]*rot_mat[index]*scale_mat[index];
}

void Planetoids::rotate(glm::vec3 direction, float angle, int index)
{
    rot_mat[index] = glm::rotate(rot_mat[index], angle, direction);
    matrix_update(index);
}

void Planetoids::place(glm::vec3 pos, int index)
{
    trans_mat[index] = glm::translate(pos);
    matrix_update(index);
}

void Planetoids::translate(glm::vec3 d, int index)
{
    trans_mat[index] = glm::translate(trans_mat[index], d);
    matrix_update(index);
}

void Planetoids::update(float dt)
{
    glm::vec3 v,ds;
    for(int i = 0; i < nb; i++){
        v = velocity[i]*normalize(cross(rot_axis[i], position[i]));
        position[i] = position[i] + dt*v;
        place(position[i], i);
    }
}

void Planetoids::draw()
{
    glUseProgram(program);
    glBindTexture(GL_TEXTURE_2D, texture);
    for(int i = 0; i < nb; i++){
        glUniformMatrix4fv(glGetUniformLocation(program, "mdl_matrix"), 1, GL_FALSE, glm::value_ptr(matrix[i]));
        DrawModel(m, program, "in_position", "in_normal", "in_tex_coord");
    }        
}
