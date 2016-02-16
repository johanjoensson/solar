#pragma once

#include "body.h"

#include <glm/glm.hpp>

using namespace glm;

class Sun : public Body 
{
    private:
    public:
        vec3 emit_color;
        vec3 sun_position;
        float specularExponent;
        virtual void draw();
        Sun(Model*, const char* tex, GLuint shader);
};
