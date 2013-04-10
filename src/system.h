#ifndef SYSTEM_H
#define SYSTEM_H

#include "SDL_util.h"
#include "body.h"
#include "camera.h"



class System {
    private:
       
    public:
        enum CUSTOM_CODES{
            CUSTOM_TIMER = 1	
        } custom_codes;

        struct Cel_bodies{
            private:
                vec3 rk4_accel(float h, vec3 k, Cel_bodies *universe);
                vec3 rk4_velocity(float h, vec3 acc);
                void rk4_gravity(float dt, Cel_bodies *universe);
            public:
                Body *planet;
                Cel_bodies *next;
                
                vec3 acceleration(Cel_bodies *second, float h, vec3 first_k, vec3 second_k);
                void calculate_k1();
                void calculate_k2(float h);
                void calculate_k3(float h);
                void calculate_k4(float h);
                void update_gravity(float dt);
                void reset_k();
                void calculate_slopes(float dt);

                void add_planet(Body*);
                void remove_planet(Body*);
                void clear_list();
                void update(float);

                Cel_bodies();
                ~Cel_bodies();
        }bodies;

        Camera cam;

        
        static void event_handler(SDL_Event event);

        System();
};
#endif //SYSTEM_H
