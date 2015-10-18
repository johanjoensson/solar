#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "loadobj.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Object {
    protected:
        glm::mat4 rot_mat;
        glm::mat4 trans_mat;
        glm::mat4 scale_mat;

        // För att skala objekt
        float scale;

        SDL_Surface* surface;
    public:
        Model *m;

        glm::mat4 matrix;
        glm::vec3 position;

        // För att sätta skalan
        void set_scale(float);
        // För att returnera skala
        float get_scale(); 

        float reflectivity;
        GLuint texture;

        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        void place(glm::vec3 pos);
        void update();
        void draw(int);

        Object();
        Object(const char*);
        Object(Model*, const char*);
        Object(const char*, const char*);
};
