#define MAINFILE

#include <math.h>
#include "GL_utilities.h"
#include "SDL_util.h"
#include "body.h"
#include "loadobj.h"

// Globals
GLfloat a = 0.0;

// Reference to shader program
GLuint program;

// Model
Model *m;

void init(void)
{
	dumpInfo();

    //Object o = Object("../res/planet.obj");
    m = LoadModelPlus("bunnyplus.obj");

	// GL inits
	glClearColor(0.5,0.2,0.2,1.0);
	glEnable(GL_DEPTH_TEST);

	// Load and compile shader
	program = loadShaders("test.vert", "test.frag");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawModel(m, program, "in_Position",NULL,NULL);
	
	SDL_GL_SwapBuffers();
}

Uint32 OnTimer(Uint32 interval, void* param)
{
	a += 0.1;

    // För att få bort varningar
    param = NULL;
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)CUSTOM_TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

int main()
{
	init_SDL();
	set_sdl_display_func(&display);
	init();
	SDL_TimerID timer_id;
	timer_id = SDL_AddTimer(17, &OnTimer, NULL);
	if(timer_id == NULL){
		fprintf(stderr, "Error setting timer function: %s", SDL_GetError());
	}

	inf_loop();
}
