#ifndef OBJECT_H
#define OBJECT_H

#include<GL/gl.h>
#include"VectorUtils3.h"
#include "loadobj.h"

class Object {
    public:
        Model *m;
        mat4 matrix;
        float reflectivity;
        int texture;
        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        void update();
        void draw();
        void print_matrix();
        Object();
        Object(const char*);
        //~Object();
};

#endif
