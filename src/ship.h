#ifndef SHIP_H
#define SHIP_H

#include "object.h"
#include <GL/gl.h>

class Ship: public Object {
    private:
    public:
        void draw(int);
        Ship(const char* model, const char *texture) : Object(model, texture){};
};
#endif
