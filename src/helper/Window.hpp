#pragma once

#include <string>
#include <SDL2/SDL.h>

class Window 
{
    private:
        SDL_Window* window_;

    public:
        Window(const std::string &title, int width, int height, Uint32 flags);

        ~Window();

        // No copy constructor
        Window(const Window& other) = default;

        // No copy assignment
        Window& operator=(const Window& other) = delete;

        // No move constructor
        Window(const Window&& other) = delete;

        // No move assignment
        Window& operator=(const Window&& other) = delete;

        // Set the title, return reference to the window
        Window& SetTitle(const std::string &title);

        // Get a pointer to the window
        SDL_Window *Get() const;

        // Get the title of the window
        std::string GetTitle() const;

        // Maximize the window. Return reference to the window
        Window& Maximize();

        // Minimize the window. Return reference to the window
        Window& Minimize();

        // Resize window
        Window& Resize(int w, int h);

        // Swap buffers
        Window& Swap();

        // Get the width of the window
        int getWidth() const;

        // Get the height of the window
        int getHeight() const;

        // Handle a SDL window event
        void handleEvent(SDL_Event e);
};
