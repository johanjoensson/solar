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
            Body *planet;
            Cel_bodies *next;

            void add_planet(Body*);
            void remove_planet(Body*);
	    void clear_list();
            void update();
        }bodies;

        Camera cam;

        
        static void event_handler(SDL_Event event);

        System();
};
#endif //SYSTEM_H
