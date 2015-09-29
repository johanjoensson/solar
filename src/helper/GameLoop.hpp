#pragma once

#include<SDL2/SDL.h>
#include "Window.hpp"

using EventFunc = void(*)(SDL_Event);

class GameLoop
{
    public:
        GameLoop(Window &window, EventFunc user_handler, EventFunc keypress_handler, EventFunc mouse_handler);
        void run();
    private:
        SDL_Event event_;
        EventFunc user_handler_;
        EventFunc keypress_handler_;
        EventFunc mouse_handler_;
        Window window_;
        void handle_event(SDL_Event event);
};
