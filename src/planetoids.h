#ifndef PLANETOIDS_H
#define PLANETOIDS_H

#include "VectorUtils3.h"
#include "body.h"

class Planetoids: public Body {
    private:
        int program;

        void matrix_update(int);

        mat4* rot_mat;
        mat4* trans_mat;
        mat4* scale_mat;

        vec3* rot_axis;

        // För att skala objekt
        float* scale;

    public:

        int nb;
        mat4* matrix;
        vec3* position;
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


        void rotate(vec3 direction, float angle, int index);
        void translate(vec3, int index);
        void place(vec3 pos, int index);


        void update(float);
        void draw();

};
#endif //PLANETOIDS_H
