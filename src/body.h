#pragma once

#include "object.h"
#include <glm/vec3.hpp>

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
        glm::vec3 velocity;

        void update(float);
        virtual void draw(int);
        Body(const char*);
        Body(Model*, const char*);
        Body();

#ifdef GRAV_OPT
/******************************************************************************
 * Hjälplutningar till den "optimerade" implementationen av rk4-integration
 *****************************************************************************/
        glm::vec3 kv1;
        glm::vec3 kv2;
        glm::vec3 kv3;
        glm::vec3 kv4;

        glm::vec3 kr1;
        glm::vec3 kr2;
        glm::vec3 kr3;
        glm::vec3 kr4;
#endif //GRAV_OPT
};
