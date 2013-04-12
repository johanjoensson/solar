#include <math.h>
#include <SDL.h>
#include "GL_utilities.h"
#include "SDL_util.h"
#include "body.h"
#include "loadobj.h"
#include "camera.h"
#include "VectorUtils3.h"
#include "system.h"
#include "LoadTGA.h"
#include "spacebox.h"
#include<stdio.h>

#define near 1
#define far 300
#define right 1
#define left -1
#define bottom -1
#define top 1

// Globals
GLfloat a = 0.0;
const SDL_VideoInfo* info; 

// Reference to shader program
GLuint program;
System sys;

mat4 projection_matrix;

void handle_keypress(SDL_Event event);
void handle_mouse(SDL_Event event);
static void event_handler(SDL_Event event);
void check_keys();

void init(void)
{
	dumpInfo();
    // GL inits
	glClearColor(0.5,0.2,0.2,1.0);
	glEnable(GL_DEPTH_TEST);

	// Load and compile shader
	program = loadShaders("src/test.vert", "src/test.frag");
	printError("error loading shaders");

    sys = System(program, 20);

    // Set Texture units
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

    // Hide the cursor 
    SDL_ShowCursor(0);
    
    // Lock cursor to this program
    SDL_WM_GrabInput( SDL_GRAB_ON );

    // Create and upload projection matrix
    projection_matrix = frustum(left, right, bottom, top, near, far);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_TRUE, projection_matrix.m);
	printError("error loading projection");

    //glUniform1i(glGetUniformLocation(program, "spacebox"), 0);
}

void display(void)
{
    // FIXME check_keys ska kanske inte ligga här
    check_keys();
	printError("pre display");

    sys.update(20);

	// clear the screen
    sys.draw(program); 
	printError("draw error");

	SDL_GL_SwapBuffers();
}


/******************************************************************************
 * Anropa INGA som helst funktioner i denna!
 * Det blir bara fel utan någon som helst anledning!
 * Denna sätter timer-eventet och gör inget annat!
 *****************************************************************************/
Uint32 OnTimer(Uint32 interval, void* param)
{
	a += 0.1;

    // För att få bort varningar
    param = NULL;
    param = param;

	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)System::CUSTOM_TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

int main()
{
	init_SDL();
	set_sdl_display_func(&display);
    set_event_handler(&event_handler);
	init();
	SDL_TimerID timer_id;
	timer_id = SDL_AddTimer(20, &OnTimer, NULL);
	if(timer_id == NULL){
		fprintf(stderr, "Error setting timer function: %s", SDL_GetError());
	}

	inf_loop();
}

void handle_keypress(SDL_Event event)
{
	switch(event.key.keysym.sym){
		case SDLK_ESCAPE:
		case SDLK_q:
			exit_prog(0);
			break;
		default:
			break;
	}
}

void handle_mouse(SDL_Event event)
{
    info = SDL_GetVideoInfo();
    int width = info->current_w; 
    int height = info->current_h; 

    sys.c.change_look_at_pos(event.motion.xrel,event.motion.y,width,height);
}

void event_handler(SDL_Event event){
	switch(event.type){
		case SDL_VIDEORESIZE: 
			resize_window(event);
			break;
		case SDL_KEYDOWN:
			handle_keypress(event);
			break;
		case SDL_QUIT:
			exit_prog(0);
			break;
		case SDL_USEREVENT:
			handle_userevent(event);
			break;
        case SDL_MOUSEMOTION:
            handle_mouse(event);
            break;
		default:
			break;
	}
}

void check_keys(){
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if(keystate[SDLK_w])
    {
        sys.c.forward(0.1);
    } else if(keystate[SDLK_s])
    {
        sys.c.forward(-0.1);
    }
    if(keystate[SDLK_a])
    {
        sys.c.strafe(0.1);
    } else if(keystate[SDLK_d])
    {
        sys.c.strafe(-0.1);
    }
}
