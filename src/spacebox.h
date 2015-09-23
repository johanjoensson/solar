#pragma once

#include "object.h"

class Spacebox : public Object {
    public:
        void draw(int);
        Spacebox();
        Spacebox(const char*, const char*);
};
