#include "SDL_util.hpp"
#include "SDL_exception.hpp"

SDL::SDL(Uint32 flags)
{
    if(SDL_Init(flags) != 0){
        throw SDL_exception("SDL_Init");
    }
}

SDL::~SDL()
{
    SDL_Quit();
}

void (*handle_event)(SDL_Event event);

void set_event_handler(void (*event_func)(SDL_Event event))
{
    handle_event = event_func;
}

void inf_loop()
{
	SDL_Event event;

	while(1){
		while(SDL_PollEvent(&event)){
			(*handle_event)(event);	
		}
	}
}
