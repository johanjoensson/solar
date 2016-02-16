#pragma once

#include "object.h"

class Spacebox : public Object {
    public:
        void draw();
        Spacebox() = default;
        Spacebox(const char* model, const char* tex, GLuint shader);
};
