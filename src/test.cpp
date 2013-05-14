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
#include "cel_bodies.h"
#include <stdio.h>
#include <unistd.h>

#define near 1
#define far 300
#define right 1
#define left -1
#define bottom -1
#define top 1

#define MAX_SPEED 20
#define MAX_SIMULATION_SPEED 15
#define MAX_DISTANCE 400


// Globals
const SDL_VideoInfo* info; 

// Reference to shader program
GLuint program;
System sys;
int speed = 4;
int simulation_speed = 1;

mat4 projection_matrix;

void handle_keypress(SDL_Event event);
void handle_mouse(SDL_Event event);
static void event_handler(SDL_Event event);
void check_keys();
void handle_userevent(SDL_Event event);

void init(int argc, char *argv[])
{
	dumpInfo();
    // GL inits
	glClearColor(0.5,0.2,0.2,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	// Load and compile shader
	program = loadShaders("src/test.vert", "src/test.frag");
	printError("error loading shaders");

    int nsun = 0;
    int nplanet = 0;
    long p_mass_range = 2E6;
    long s_mass_range = 6E10;
    float p_vel_range = 2;
    int p_pos_range = 0;
    int c;
    while ((c = getopt(argc, argv, "p:sm:n:v:r:")) != -1) {
        switch(c) {
            case 'p':
                nplanet = atoi(optarg);
                break;
            case 's':
                nsun = 1;
                break;
            case 'm':
                p_mass_range = atol(optarg);
                break;
            case 'n':
                s_mass_range = atol(optarg);
                break;
            case 'v':
                p_vel_range = atof(optarg);
                break;
            case 'r':
                p_pos_range = atoi(optarg);
                break;
        }
    }

    if (nsun == 0 && nplanet == 0) {
        sys = System(program);
    } else {
        sys = System(program, nplanet, nsun, p_mass_range, s_mass_range, p_vel_range, p_pos_range);
    }

    sys.f = Frustum(near, far, bottom, top, left, right);

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

void update(int interval)
{
    check_keys();
    sys.update(interval);
}

void display(void)
{
	printError("pre display");
    sys.draw(program); 
	// clear the screen
	printError("draw error");

	SDL_GL_SwapBuffers();
}


/******************************************************************************
 * Anropa INGA som helst funktioner i denna!
 * Det blir bara fel utan någon som helst anledning!
 * Denna sätter timer-eventet och gör inget annat!
 *****************************************************************************/
Uint32 display_timer(Uint32 interval, void* param)
{
    // För att få bort varningar
    param = NULL;
    param = param;

	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)System::DISPLAY_TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

Uint32 update_timer(Uint32 interval, void* param)
{
    // För att få bort varningar
    param = NULL;
    param = param;

	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)System::UPDATE_TIMER;
	event.user.data1 = (void*) interval;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

Uint32 clean_timer(Uint32 interval, void* param)
{
    // För att få bort varningar
    param = NULL;
    param = param;

	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)System::CLEAN_TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

int main(int argc, char* argv[])
{
	init_SDL();
    set_event_handler(&event_handler);
	init(argc, argv);
	SDL_TimerID timer_id;
	timer_id = SDL_AddTimer(20, &display_timer, NULL);
	timer_id = SDL_AddTimer(5, &update_timer, NULL);
	timer_id = SDL_AddTimer(1000, &clean_timer, NULL);
	if(timer_id == NULL){
		fprintf(stderr, "Error setting timer function: %s", SDL_GetError());
	}

	inf_loop();
    sys.bodies.clear_list();
}

void handle_keypress(SDL_Event event)
{
	switch(event.key.keysym.sym){
		case SDLK_ESCAPE:
		case SDLK_q:
			exit_prog(0);
			break;
        case SDLK_UP:
            speed++;
            if(speed > MAX_SPEED){
                speed = MAX_SPEED;
            }
            break;
        case SDLK_DOWN:
            speed--;
            if(speed < 1){
                speed = 1;
            }
            break;
        case SDLK_LEFT:
            simulation_speed--;
            if(simulation_speed < 1){
                 simulation_speed = 1;
            }
            break;
        case SDLK_RIGHT:
            simulation_speed++;
            if(simulation_speed > MAX_SIMULATION_SPEED){
                 simulation_speed = MAX_SIMULATION_SPEED;
            }
            break;
        case SDLK_g: 
            {
                SDL_GrabMode mode = SDL_WM_GrabInput(SDL_GRAB_QUERY);
                if(mode == SDL_GRAB_ON) {
                    SDL_WM_GrabInput(SDL_GRAB_OFF);
                }
                else {
                    SDL_WM_GrabInput(SDL_GRAB_ON);
                }
            }
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
    sys.ship.handle_movement(event.motion.xrel,event.motion.yrel,width,height);
}

void event_handler(SDL_Event event)
{
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

void check_keys()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if(keystate[SDLK_w]) {
        sys.c.forward(0.1*speed);
    } else if(keystate[SDLK_s]) {
        sys.c.forward(-0.1*speed);
    }
    if(keystate[SDLK_a]) {
        sys.c.strafe(0.1*speed);
    } else if(keystate[SDLK_d]) {
        sys.c.strafe(-0.1*speed);
    }
}

void handle_userevent(SDL_Event event)
{
    switch(event.user.code){
        case (int)System::DISPLAY_TIMER:
            display();
            break;
        case (int)System::UPDATE_TIMER:
            update(simulation_speed * *((Uint32*)(&event.user.data1)));
            break;
        case (int)System::CLEAN_TIMER:
            sys.clean(MAX_DISTANCE);
            break;
        default:
            break;
    }
}
