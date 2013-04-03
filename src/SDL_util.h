#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#ifndef MAINFILE
    #define EXTERN
#else
    #define EXTERN extern
#endif

#include <SDL.h>

enum CUSTOM_CODES{
	CUSTOM_TIMER = 1	
<<<<<<< HEAD
} EXTERN custom_codes;
=======
} extern custom_codes;
>>>>>>> 09f1f6100a852b909073a90e77ce26d6d6f86e5a

void set_sdl_display_func(void (*display_func)(void));

void init_SDL();

void inf_loop();

#endif //SDL_UTIL_H
