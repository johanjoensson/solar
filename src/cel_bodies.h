#pragma once

#include "body.h"
#include <glm/glm.hpp>

/******************************************************************************
 * Struct som innehåller en länkad lista till alla gravitationellt bundna
 * objekt i världen.
 * Samt funktioner för att hantera denna lista
 *****************************************************************************/

struct Cel_bodies{
    private:
        glm::vec3 force(Cel_bodies *second, float h, glm::vec3 first_k, glm::vec3 second_k);
        void calculate_k1();
        void calculate_k2(float h);
        void calculate_k3(float h);
        void calculate_k4(float h);
        void update_gravity(float dt);
        void reset_k();
        void calculate_slopes(float dt);

    public:
        Body *planet;
        Cel_bodies *next;

        void add_planet(Body*);
        void remove_planet(Body*);
        void clear_list();
        void update(float);

        Cel_bodies();
};
