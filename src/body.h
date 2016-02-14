#pragma once

#include "object.h"
#include <glm/vec3.hpp>

class Body: public Object {
    private:
    public:
        float spin_x = 0;
        float spin_y = 0;
        float spin_z = 0;
        float mass = 1;
        void set_radius(float);
        float get_radius();
        glm::vec3 velocity;

        void update(float);
        virtual void draw(int);
        Body(const char*);
        Body(Model*, const char*);
        Body();

/******************************************************************************
 * Hjälplutningar till den "optimerade" implementationen av rk4-integration
 *****************************************************************************/
        glm::vec3 kv1 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kv2 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kv3 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kv4 = glm::vec3(0.0, 0.0, 0.0);

        glm::vec3 kr1 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kr2 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kr3 = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 kr4 = glm::vec3(0.0, 0.0, 0.0);
};
