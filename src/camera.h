#ifndef CAMERA_H
#define CAMERA_H

#include"VectorUtils3.h"

class Camera {
    private:
        void upload();
        int program;
    public:
        vec3 position;
        vec3 look_at_pos;

        vec3 up;
        mat4 matrix;

        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        float radius;
        void point_to(vec3 pos);
        Camera(int program);
        Camera();
        void print_matrix();
};

#endif
