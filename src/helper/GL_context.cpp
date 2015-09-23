#include "GL_context.hpp"
#include "SDL_exception.hpp"
#include <GL/gl.h>

GL_context::GL_context(Window &window)
{
    if((glContext_ = SDL_GL_CreateContext(window.Get())) == nullptr){
        throw SDL_exception("SDL_GL_CreateContext");
    }

    int width = window.getWidth();
    int height = window.getHeight();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, width, 0, height, -1, 1);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
}

GL_context::~GL_context()
{
    SDL_GL_DeleteContext(glContext_);
}
