#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>
#include "VectorUtils3.h"
#include "loadobj.h"

class Object {
    protected:
        mat4 rot_mat;
        mat4 trans_mat;
    public:
        Model *m;

        mat4 matrix;
        vec3 position;

        float reflectivity;
        GLuint texture;

        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        void place(vec3 pos);
        void update();
        void draw(int);
        void print_matrix();

        Object();
        Object(const char*);
        Object(const char*, const char*);
};

#endif
