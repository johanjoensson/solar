#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "loadobj.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Object {
    protected:
        glm::mat4 rot_mat = glm::mat4();
        glm::mat4 trans_mat = glm::mat4();
        glm::mat4 scale_mat = glm::mat4();

        // För att skala objekt
        float scale = 1;

        SDL_Surface* surface;
        Model *m;

        glm::mat4 matrix = glm::mat4();

        float reflectivity = 1;

        GLuint texture;
        GLuint shader;
    public:
        glm::vec3 position;

        // För att sätta skalan
        void set_scale(float);
        // För att returnera skala
        float get_scale(); 

        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        void place(glm::vec3 pos);
        void update();
        virtual void draw();

        Object() = default;
        Object(Model*, const char* tex, GLuint shader);
        Object(const char* model, const char* tex, int shader);
};
