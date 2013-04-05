#ifndef SYSTEM_H
#define SYSTEM_H

#include "body.h"


class System {
    public:
        struct Cel_bodies{
            Body planet;
            Cel_bodies *next;

            void add_planet(Body*);
            void remove_planet(Body*);
            void update();
        }bodies;
        
        System();
};
#endif //SYSTEM_H
