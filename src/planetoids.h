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

        // För att skala objekt
        float* scale;

    public:

        int nb;
        mat4* matrix;
        vec3* position;
        vec3* velocity;
        float* mass;
        float* spin_x;
        float* spin_y;
        float* spin_z;
        float* reflectivity;

        Planetoids();
        Planetoids(int, const char*);
        Planetoids(int, const char*, const char*);
        Planetoids(int, const char*, const char*,  const char*, const char*);


        void rotate(char direction, float angle, int index);
        void translate(float dx, float dy, float dz, int index);
        void place(vec3 pos, int index);


        void update(float);
        void draw();

};
#endif //PLANETOIDS_H
