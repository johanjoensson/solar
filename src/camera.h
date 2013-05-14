#ifndef CAMERA_H
#define CAMERA_H

#include"VectorUtils3.h"

class Camera {
    private:
        int program;

        // x används för att musen inte ska fastna i kanterna på 
        // fönstret
        int x;
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
	float cam_position[3];
        void point_to(vec3 pos);

        /*************************************************************
         * change_look_at_pos:
         * Tar xrel från MouseMotionEvent och y som absolut koordinat
         * width, height är storlek på nuvarande fönster
         * Sätter look_at_pos därefter
         * **********************************************************/
        void change_look_at_pos(int xrel, int y, int width, int height);
        void upload();
        Camera(int program);
        Camera();
        void print_matrix();
};

#endif
