#pragma once

#include "body.h"
#include <glm/glm.hpp>

class Planetoids: public Body {
    private:
        int program;

        void matrix_update(int);

        glm::mat4* rot_mat;
        glm::mat4* trans_mat;
        glm::mat4* scale_mat;

        glm::vec3* rot_axis;

        // För att skala objekt
        float* scale;

    public:

        int nb;
        glm::mat4* matrix;
        glm::vec3* position;
        float* velocity;
        float* mass;
        float* spin_x;
        float* spin_y;
        float* spin_z;
        float* reflectivity;

        Planetoids();
        Planetoids(int, const char*);
        Planetoids(int, const char*, const char*);
        Planetoids(int, float, const char*, const char*,  const char*, const char*);


        void rotate(glm::vec3 direction, float angle, int index);
        void translate(glm::vec3, int index);
        void place(glm::vec3 pos, int index);


        void update(float);
        void draw();

};
