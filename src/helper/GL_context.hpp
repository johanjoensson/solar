#pragma once

#include "Window.hpp"
#include <SDL2/SDL.h>


class GL_context
{
    private:
        SDL_GLContext glContext_;
    public:
        GL_context(Window &window);
        ~GL_context();

        // No copy constructor
        GL_context(GL_context &other) = delete;

        // No copy assignment
        GL_context& operator=(GL_context &other) = delete;

        // No move constructor
        // TODO: Tillåta move constructors kanske
        GL_context(GL_context &&other) = delete;

        // No move assignment
        // TODO: Tillåta move assigment kanske
        GL_context& operator=(GL_context &&other) = delete;
};
