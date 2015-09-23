#include "Timer.hpp"
#include "SDL_exception.hpp"

Timer::Timer(Uint32 delay, SDL_TimerCallback callback, void* param)
{
    if((timer_ = SDL_AddTimer(delay, callback, param)) == 0){
       throw SDL_exception("SDL_AddTimer"); 
    }
}
