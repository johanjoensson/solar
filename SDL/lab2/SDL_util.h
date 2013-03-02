#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include <SDL.h>

enum CUSTOM_CODES{
	CUSTOM_TIMER = 1	
} custom_codes;

void set_sdl_display_func(void (*display_func)(void));

void init_SDL();

void inf_loop();

#endif //SDL_UTIL_H
