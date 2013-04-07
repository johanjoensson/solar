#ifndef CAMERA_H
#define CAMERA_H

#include"VectorUtils3.h"

class Camera {
    private:
        int program;
    public:
        vec3 position;
        vec3 look_at_pos;

        vec3 up;
        mat4 matrix;

        void rotate(char direction, float angle);
        void translate(float dx, float dy, float dz);
        void forward(float d);
        void strafe(float d);
        void update();
        float radius;
        void point_to(vec3 pos);
        void upload();
        Camera(int program);
        Camera();
        void print_matrix();
};

#endif
