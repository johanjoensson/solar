#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include "body.h"
#include "loadobj.h"
#include "camera.h"
#include "system.h"
#include "spacebox.h"
#include "cel_bodies.h"
#include "planetoids.h"

#include "helper/SDL_util.hpp"
#include "helper/Window.hpp"
#include "helper/GL_context.hpp"
#include "helper/Timer.hpp"
#include "helper/GameLoop.hpp"
#include "helper/GLShader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define near 1
#define far 300
#define right 1
#define left -1
#define bottom -1
#define top 1

#define MAX_SPEED 20
#define MAX_SIMULATION_SPEED 15
#define MAX_DISTANCE 400


// Reference to shader program
GLuint program;
System sys;
int speed = 2;
int simulation_speed = 1;
int screenshot_nr = 0;

mat4 projection_matrix;

void handle_keypress(SDL_Event event);
void handle_mouse(SDL_Event event);
void check_keys();
void handle_userevent(SDL_Event event);

void display_help()
{
    std::cerr << "How to use the different flags:\n";
    std::cerr << "-s\tcreates a sun\n";
    std::cerr << "-p nb\tcreates nb planets (randomized attributes)\n";
    std::cerr << "-r nb\tset the maximum distance from origin in which planets can be created\n";
    std::cerr << "-m nb\tsets the maximum mass of planets\n";
    std::cerr << "-n nb\tsets the maximum mass of the sun\n";
    std::cerr << "-v nb\tsets the maximum initial velocity of the planets\n";
    std::cerr << "-a nb\tcreates an asteroid belt with nb asteroids\n";
    std::cerr << "\nUseful things:\n";
    std::cerr << "Pressing 'g' while running the program will release the keyboard and mouse, allowing you to use them for other things.\n";
    std::cerr << "Pressing 'p' while running the program will take a snapshot of the program running.\n";
    std::cerr << "\nThank you for reading this, now go and play god/alien invader!\n";
}

void init(int argc, char** argv)
{
	dumpInfo();
    // GL inits
	glClearColor(0.5,0.2,0.2,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Load and compile shader
	program = LoadShader("src/solar.vert", "src/solar.frag");
	printError("error loading shaders");

    int nsun = 0;
    int nplanet = 0;
    long p_mass_range = 2E6;
    long s_mass_range = 6E10;
    float p_vel_range = 2;
    int p_pos_range = 0;
    int nasteroid = 0;
    int c;
    while ((c = getopt(argc, argv, "p:sm:n:v:r:ha:")) != -1) {
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
            case 'a':
                nasteroid = atoi(optarg);
                break;
            case 'h':
                display_help();
                exit(0);

        }
    }

    if (nsun == 0 && nplanet == 0) {
        sys = System(program);
    } else {
        sys = System(program, nplanet, nsun, nasteroid, p_mass_range, s_mass_range, p_vel_range, p_pos_range);
    }

    sys.f = Frustum(near, far, bottom, top, left, right);

    // Set Texture units
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

    // Lock cursor to this program
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create and upload projection matrix
    projection_matrix = glm::frustum(left, right, bottom, top, near, far);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	printError("error loading projection");
}

void update(int interval)
{
    check_keys();
    sys.update(interval);
}

void display(void *window_void_ptr)
{
    Window *window = static_cast<Window *>(window_void_ptr);
	printError("pre display");
    sys.draw(program); 
	// clear the screen
	printError("draw error");

    window->Swap();
}



/******************************************************************************
 * Anropa INGA som helst funktioner i denna!
 * Det blir bara fel utan någon som helst anledning!
 * Denna sätter timer-eventet och gör inget annat!
 *****************************************************************************/
Uint32 display_timer(Uint32 interval, void* param)
{
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)System::DISPLAY_TIMER;
	event.user.data1 = param;
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
	event.user.data1 = (void*) (intptr_t) interval;
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

void handle_keypress(SDL_Event event)
{
	switch(event.key.keysym.sym){
		case SDLK_ESCAPE:
		case SDLK_q:
			exit(0);
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
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            break;
        case SDLK_h: 
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            break;
		default:
			break;
	}
}

void handle_mouse(SDL_Event event)
{
    // TODO(gustav) Inte hårdkoda dessa
    int width = 1024; 
    int height = 768; 

    sys.c.change_look_at_pos(event.motion.xrel,event.motion.y,width,height);
    sys.ship.handle_movement(event.motion.xrel,event.motion.yrel,width,height);
}

void check_keys()
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_W]) {
        sys.c.forward(0.1*speed);
    } else if(keystate[SDL_SCANCODE_S]) {
        sys.c.forward(-0.1*speed);
    }
    if(keystate[SDL_SCANCODE_A]) {
        sys.c.strafe(0.1*speed);
    } else if(keystate[SDL_SCANCODE_D]) {
        sys.c.strafe(-0.1*speed);
    }
}

void handle_userevent(SDL_Event event)
{
    switch(event.user.code){
        case (int)System::DISPLAY_TIMER:
            display(event.user.data1);
            break;
        case (int)System::UPDATE_TIMER:
            update(simulation_speed * *((Uint32*)(intptr_t)(&event.user.data1)));
            break;
        case (int)System::CLEAN_TIMER:
            sys.clean(MAX_DISTANCE);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    // Ta bort Haptic support, som verkar saknas på gentoo
    SDL sdl(SDL_INIT_EVERYTHING ^ SDL_INIT_HAPTIC);
    // TODO(gustav) byta till inte hårdkodade värden
    Window window("Solar", 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    GL_context glContext(window);
	init(argc, argv);

    Timer disp_timer(30, display_timer, &window);
    Timer upd_timer(5, update_timer, NULL);
    Timer cln_timer(1000, clean_timer, NULL);

    GameLoop game(window, handle_userevent, handle_keypress, handle_mouse);

    game.run();

    sys.bodies.clear_list();
}

