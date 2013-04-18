/******************************************************************************
 * Klassen System ska ansvara för att hantera objekten i världen.
 * Den styr gravitationen, och räknar ut hur objekt rör sig.
 *****************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H


#include "SDL_util.h"
#include "body.h"
#include "camera.h"
#include "spacebox.h"
#include "cel_bodies.h"


/******************************************************************************
 * Klassen system
 *****************************************************************************/
class System {
    public:
        enum CUSTOM_CODES{
            CUSTOM_TIMER = 1	
        } custom_codes;

        Camera cam;
        
        void update(float);
        void draw(int);

        Body b;
        Spacebox s;
        Camera c;
        
        /**********************************************************************
         * OBS: första objektet i listan är ingen planet i systemet,
         * det är en sentinel, som finns för att förenkla implementationen av
         * add/remove_planet.
         *
         * bodies.next är första objektet i listan!
         *********************************************************************/
        Cel_bodies bodies;

        System();
        System(int);

};
#endif //SYSTEM_H
