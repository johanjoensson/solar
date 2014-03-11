#ifndef CEL_BODIES_H
#define CEL_BODIES_H

#include "body.h"

/******************************************************************************
 * Struct som innehåller en länkad lista till alla gravitationellt bundna
 * objekt i världen.
 * Samt funktioner för att hantera denna lista
 *****************************************************************************/

struct Cel_bodies{
    private:
#ifndef GRAV_OPT 
        /* GRAV_OPT sätts i body.h och styr vilken form av rk4-integrator vi kompilerar
         * och kör */
        /******************************************************************************
         * Dessa funktioner används för den enklare, men något långsammare
         * implementationen av rk4-integratorn
         *****************************************************************************/
        vec3 rk4_accel(float h, vec3 k, Cel_bodies *universe);
        vec3 rk4_velocity(float h, vec3 acc);
        void rk4_gravity(float dt, Cel_bodies *universe);
#else // GRAV_OPT
        /******************************************************************************
         * Dessa funktioner används för den mer komplicerade, men något snabbare
         * implementationen av rk4-integratorn.
         *****************************************************************************/
        vec3 force(Cel_bodies *second, float h, vec3 first_k, vec3 second_k);
        void calculate_k1();
        void calculate_k2(float h);
        void calculate_k3(float h);
        void calculate_k4(float h);
        void update_gravity(float dt);
        void reset_k();
        void calculate_slopes(float dt);


#endif // GRAV_OPT
    public:
        Body *planet;
        Cel_bodies *next;

        void add_planet(Body*);
        void remove_planet(Body*);
        void clear_list();
        void update(float);

        Cel_bodies();
};

#endif // CEL_BODIES_H
