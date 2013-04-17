#ifndef BODY_H
#define BODY_H

#include "object.h"
#include "VectorUtils3.h"

/******************************************************************************
 * Styr vilken implementation av rk4-integration vi kör.
 *****************************************************************************/
#define GRAV_OPT
//#undef GRAV_OPT

class Body: public Object {
    private:
    public:
        float spin_x;
        float spin_y;
        float spin_z;
        float mass;
        void set_radius(float);
        float get_radius();
        vec3 velocity;

        void update(float);
        void draw(int);
        Body(const char*);
        Body(const char*, const char*);
        Body();

#ifdef GRAV_OPT
/******************************************************************************
 * Hjälplutningar till den "optimerade" implementationen av rk4-integration
 *****************************************************************************/
        vec3 kv1;
        vec3 kv2;
        vec3 kv3;
        vec3 kv4;

        vec3 kr1;
        vec3 kr2;
        vec3 kr3;
        vec3 kr4;
#endif //GRAV_OPT
};
#endif
