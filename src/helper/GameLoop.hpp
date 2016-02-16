#pragma once

#include<SDL2/SDL.h>
#include "Window.hpp"

using EventFunc = void(*)(SDL_Event);
using EventWinFunc= void(*)(SDL_Event, Window&);

class GameLoop
{
    public:
        GameLoop(Window &window, EventFunc user_handler, EventWinFunc keypress_handler, EventWinFunc mouse_handler);
        void run();
    private:
        int running = 1;
        SDL_Event event_;
        EventFunc user_handler_;
        EventWinFunc keypress_handler_;
        EventWinFunc mouse_handler_;
        Window window_;
        void handle_event(SDL_Event event);
};
