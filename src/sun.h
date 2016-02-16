#pragma once

#include "body.h"

#include <glm/glm.hpp>

using namespace glm;

class Sun : public Body 
{
    private:
        GLuint planet_shader;
    public:
        Sun(Model*, const char* tex, GLuint shader, GLuint planet_shader);

        vec3 emit_color;
        float specularExponent;
        virtual void draw();
};
