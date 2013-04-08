#ifndef BODY_H
#define BODY_H

#include "object.h"
#include "VectorUtils3.h"

class Body: public Object {
    private:
    public:
        float spin_x;
        float spin_y;
        float spin_z;
        float mass;
        float radius;
        vec3 velocity;

        void update(float);
        void draw(int);
        Body(const char*);
        Body(const char*, const char*);
        Body();
};
#endif
