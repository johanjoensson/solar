/******************************************************************************
 * Klassen System ska ansvara för att hantera objekten i världen.
 * Den styr gravitationen, och räknar ut hur objekt rör sig.
 *****************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H


#include "SDL_util.h"
#include "body.h"
#include "camera.h"
#include "frustum.h"
#include "spacebox.h"
#include "cel_bodies.h"
#include "ship.h"


/******************************************************************************
 * Klassen system
 *****************************************************************************/
class System {
    public:
        enum CUSTOM_CODES{
            CUSTOM_TIMER = 1,
            DISPLAY_TIMER = 2,
            UPDATE_TIMER = 3,	
            CLEAN_TIMER = 4	
        } custom_codes;

        Camera cam;
        
        void update(float);
        void draw(int);
        
        //Kollar om två kroppar kolliderar
        int check_collision(Body*, Body*); 
        //Uppdaterar collisioner
        void update_collisions();

        /********************************************************
         * clean(int max_distance)
         * Tar bort planeter som är långra bort än max_instance
         ********************************************************/
        void clean(int max_distance);

        /**************************************************************
         * check_distance(Body *b, int max_distance)
         * returnerar 1 om b är längre bort från origo än max_distance, 
         * 0 annars
         **************************************************************/
        int check_distance(Body* b, int max_distance);

        Body b;
        Ship ship;
        Spacebox s;
        Camera c;
        Frustum f;
        
        /**********************************************************************
         * OBS: första objektet i listan är ingen planet i systemet,
         * det är en sentinel, som finns för att förenkla implementationen av
         * add/remove_planet.
         *
         * bodies.next är första objektet i listan!
         *********************************************************************/
        Cel_bodies bodies, visible;

        System();
        System(int);
        // Konstruktor för slumpat starttillstånd
        System(int, int, int, long, long);

};
#endif //SYSTEM_H
