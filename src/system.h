#ifndef SYSTEM_H
#define SYSTEM_H

#include "SDL_util.h"
#include "body.h"
#include "camera.h"
#include "spacebox.h"

class System {
    public:
        enum CUSTOM_CODES{
            CUSTOM_TIMER = 1	
        } custom_codes;

        struct Cel_bodies{
            Body planet;
            Cel_bodies *next;

            void add_planet(Body*);
            void remove_planet(Body*);
            void update();
        }bodies;

        Camera cam;
        
        void update(Uint32);
        void draw(int);

        Body b;
        Spacebox s;
        Camera c;

        System();
        System(int);

};
#endif //SYSTEM_H
