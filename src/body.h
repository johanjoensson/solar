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


        vec3 kv1;
        vec3 kv2;
        vec3 kv3;
        vec3 kv4;

        vec3 kr1;
        vec3 kr2;
        vec3 kr3;
        vec3 kr4;
};
#endif
