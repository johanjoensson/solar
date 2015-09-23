#include<exception>
#include "SDL_exception.hpp"
#include<SDL2/SDL.h>

SDL_exception::SDL_exception(std::string function) :
    sdl_error(SDL_GetError()),
    sdl_function(function),
    what_(sdl_function + "failed: " + sdl_error) {
}

const char* SDL_exception::what() const noexcept
{
    return what_.c_str();
}
