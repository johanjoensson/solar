#ifndef SHIP_H
#define SHIP_H

#include "object.h"
#include <GL/gl.h>
#include "VectorUtils3.h"

class Ship: public Object {
    private:
        void set_rotation(vec3 rotation);
        mat4 static_rotation;
    public:
        void draw(int);
        Ship() : Object(){};
        Ship(const char* model, const char *texture);
        void handle_movement(int xrel, int yrel, int width, int height);
};
#endif
