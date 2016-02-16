#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <GL/gl.h>
#include <vector>

class Camera {
    private:
        std::vector<GLuint> shaders;

        // x används för att musen inte ska fastna i kanterna på 
        // fönstret
        int x;
        int y;
    public:
        glm::vec3 position;
        glm::vec3 look_at_pos;

        glm::vec3 up;
        glm::mat4 matrix;

        void rotate(char direction, float angle);
        void translate(glm::vec3 d_pos);
        void forward(float d);
        void strafe(float d);
        void update();
        float radius;
        float cam_position[3];
        void point_to(glm::vec3 pos);

        /*************************************************************
         * change_look_at_pos:
         * Tar xrel från MouseMotionEvent och y som absolut koordinat
         * width, height är storlek på nuvarande fönster
         * Sätter look_at_pos därefter
         * **********************************************************/
        void change_look_at_pos(int xrel, int y, int width, int height);
        void upload();
        Camera(std::vector<GLuint> shaders);
        Camera() = default;
        void print_matrix();
};
