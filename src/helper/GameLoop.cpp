#include "GameLoop.hpp"

GameLoop::GameLoop(Window &window, EventFunc user_handler, EventFunc keypress_handler, EventFunc mouse_handler):
    user_handler_(user_handler),
    keypress_handler_(keypress_handler),
    mouse_handler_(mouse_handler),
    window_(window) {}

void GameLoop::run()
{
    while(true){
        while(SDL_PollEvent(&event_)){
            handle_event(event_);
        }
    }
}

void GameLoop::handle_event(SDL_Event event)
{
    switch(event.type){
        case SDL_USEREVENT:
            user_handler_(event);
            break;
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_WINDOWEVENT:
            window_.handleEvent(event);
            break;
        case SDL_KEYDOWN:
            keypress_handler_(event);
            break;
        case SDL_MOUSEMOTION:
            mouse_handler_(event);
        default:
            break;
    }
}
