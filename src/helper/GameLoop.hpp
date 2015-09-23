#pragma once

#include<SDL2/SDL.h>

using EventFunc = void(*)(SDL_Event);

class GameLoop
{
    public:
        GameLoop(EventFunc user_handler, EventFunc keypress_handler, EventFunc mouse_handler);
        void run();
    private:
        SDL_Event event_;
        EventFunc user_handler_;
        EventFunc keypress_handler_;
        EventFunc mouse_handler_;
        void handle_event(SDL_Event event);
};
