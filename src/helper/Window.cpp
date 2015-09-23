#include "Window.hpp"
#include "SDL_exception.hpp"

Window::Window(const std::string &title, int width, int height, Uint32 flags)
{
    if((window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,  SDL_WINDOWPOS_UNDEFINED, width, height, flags)) == nullptr) {
        throw SDL_exception("SDL_CreateWindow");
    }
}

Window::~Window() {
    if(window_ != nullptr) {
        SDL_DestroyWindow(window_);
    }
}

Window& Window::SetTitle(const std::string &title)
{
    SDL_SetWindowTitle(window_, title.c_str());
    return *this;
}

std::string Window::GetTitle() const
{
    return SDL_GetWindowTitle(window_);
}

Window& Window::Maximize()
{
    SDL_MaximizeWindow(window_);
    return *this;
}

Window& Window::Minimize()
{
    SDL_MaximizeWindow(window_);
    return *this;
}

Window& Window::Resize(int w, int h)
{
    SDL_SetWindowSize(window_, w, h);
    return *this;
}

SDL_Window *Window::Get() const
{
    return window_;
}

Window &Window::Swap()
{
    SDL_GL_SwapWindow(window_);
    return *this;
}

int Window::getWidth() const
{
    int w;
    SDL_GetWindowSize(window_, &w, nullptr);
    return w;
}

int Window::getHeight() const
{
    int h;
    SDL_GetWindowSize(window_,  nullptr, &h);
    return h;
}
