#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL_util.h"

SDL_Surface* screen;
int bpp;

void (*ptdisplay)(void);
void (*handle_event)(SDL_Event event);


void set_sdl_display_func(void (*display_func)(void))
{
	ptdisplay = display_func;
	if(ptdisplay == NULL){
		fprintf(stderr, "Error setting display function\n");
	}
}

void set_event_handler(void (*event_func)(SDL_Event event))
{
    handle_event = event_func;
}

void exit_prog(int value)
{
	SDL_Quit();
	exit(value);
}

void terminate_prog( void )
{
	exit_prog(0);
}

void resize_window(SDL_Event event)
{
	int flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE;
	screen = SDL_SetVideoMode(event.resize.w, event.resize.h, bpp, flags);

	if(screen == NULL){
		fprintf(stderr, "Error resizing window: %s", SDL_GetError());
		exit_prog(1);
	}

	int new_width = screen->w;
	int new_height = screen->h;

	glViewport(0, 0, new_width, new_height);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, new_width, 0, new_height, -1, 1);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
}

/*
void handle_userevent(SDL_Event event)
{
	switch(event.user.code){
//		case (int)CUSTOM_TIMER:
				//(*ptdisplay)();
//				break;
		default:
			break;
	}	
}
*/


void inf_loop()
{
	SDL_Event event;

	while(1){
		while(SDL_PollEvent(&event)){
			(*handle_event)(event);	
		}
	}
}

void init_SDL()
{
	const SDL_VideoInfo* inf = NULL;
	
	int width, height, flags = 0;
	

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr, "Failed to initialise SDL: %s", SDL_GetError());
		exit_prog(1);
	}

	inf = SDL_GetVideoInfo();
	if(!inf){
		fprintf(stderr, "Failed to get Video Info: %s", SDL_GetError());
		exit_prog(1);
	}
	
	width = inf->current_w;
	height = inf->current_h;
	bpp = inf->vfmt->BitsPerPixel;
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF;

	screen = SDL_SetVideoMode(width >> 1, height >> 1, bpp, flags);

	if(screen == 0){
		fprintf(stderr, "Failed to set Video Mode: %s", SDL_GetError());
		exit_prog(1);
	}
	
	glViewport(0, 0, width >> 1, height >> 1);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, width >> 1, 0, height >> 1, -1, 1);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	
	atexit(terminate_prog);
}
