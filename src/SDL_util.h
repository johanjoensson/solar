#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include <SDL.h>
/******************************************************************************
 * Koder för egna event, såsom timers och liknande.
 * Används för att indentifiera ett specifikt event.
 *
 *****************************************************************************/
enum CUSTOM_CODES{
	CUSTOM_TIMER = 1	
} custom_codes;

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Sätt funktionen som anropas i inf_loop varje gång skärmen skall uppdateras
 * Inparametrar::	void (*display_func)(void)	pekare till funktionen.
 * 			får INTE returnera något
 * Returnerar:		void
 *****************************************************************************/
void set_sdl_display_func(void (*display_func)(void));

/******************************************************************************
 * Initierar SDLs system så att allt bör kunna fungera
 * Inparametrar:	-
 * Returnerar:		-
 *****************************************************************************/
void init_SDL();

/******************************************************************************
 * oändlig loop som väntar på events och hanterar dessa. Uppdaterar skärmen.
 * Inparametrar:	-
 * Returnerar:		-
 *****************************************************************************/
void inf_loop();

#ifdef __cplusplus
}
#endif

#endif //SDL_UTIL_H
