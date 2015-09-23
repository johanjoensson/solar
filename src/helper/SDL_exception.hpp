#pragma once

#include<stdexcept>
#include<string>

class SDL_exception: public std::exception {
     private:
        std::string sdl_error;
        std::string sdl_function;
        std::string what_;
    public:
        SDL_exception(std::string function);
        const char* what() const noexcept;
};
