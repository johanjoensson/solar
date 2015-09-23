#pragma once

#include<SDL2/SDL.h>

class Timer
{
    private:
        SDL_TimerID timer_;
    public:
        Timer(Uint32 delay, SDL_TimerCallback callback, void* param);
};
