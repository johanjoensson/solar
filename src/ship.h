#pragma once

#include "object.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

using namespace glm;

class Ship: public Object
{
    private:
        mat4 static_rotation;
        void set_rotation(vec3 rotation);
    public:
        void draw(int);
        Ship() : Object(){};
        Ship(const char* model, const char *texture);
        void handle_movement(int xrel, int yrel, int width, int height);
};
