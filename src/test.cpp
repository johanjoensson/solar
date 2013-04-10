#include <math.h>
#include <SDL.h>
#include "GL_utilities.h"
#include "SDL_util.h"
#include "body.h"
#include "loadobj.h"
#include "camera.h"
#include "VectorUtils3.h"
#include "system.h"
#include<iostream>
using namespace std;
#include "LoadTGA.h"
#include "spacebox.h"

#define near 1
#define far 30
#define right 1
#define left -1
#define bottom -1
#define top 1

// Globals
GLfloat a = 0.0;

// Reference to shader program
GLuint program;
Body b,p,d, e, f;
Spacebox s;
Camera c;
System sys;

mat4 projection_matrix;

void init(void)
{
	dumpInfo();
    b = Body("res/bunnyplus.obj", "res/grass.tga");
    p = Body("res/planet.obj", "res/grass.tga");
    d = Body("res/planet.obj", "res/grass.tga");
    e = Body("res/planet.obj", "res/grass.tga");
    f = Body("res/planet.obj", "res/grass.tga");
    s = Spacebox("res/skybox.obj", "res/SkyBox512.tga");
    set_event_handler(sys.event_handler);

    b.spin_y = 3.14;
    
//    b.translate(0,0,-4.0);
//    d.translate(3.0,0,-4.0);

    b.mass = 1E+11;
//    p.mass = 1E+3;
//    d.mass = 1E+2;

    b.position = vec3(0.0f, 0.0f, -6.0f);

    d.position = vec3(-4.0f, 0.0f, -6.0f);
    d.velocity = vec3(0.0, 0.0, 1.0);
    
    p.position = vec3(4.0f, 0.0f, -6.0f);
    p.velocity = vec3(0.0, 1.0, 0.0);

    e.position = vec3(4.0, 4.0, -2.0);
    e.velocity = vec3(0.0, 0.0, -2.0);

    f.position = vec3(0.0, -6.0, -19.0);
    f.velocity = vec3(0.5, 0.0, 0.0);



    sys.bodies.add_planet(&b);
    sys.bodies.add_planet(&d);
    sys.bodies.add_planet(&p);
    sys.bodies.add_planet(&e);
    sys.bodies.add_planet(&f);

	// GL inits
	glClearColor(0.5,0.2,0.2,1.0);
	glEnable(GL_DEPTH_TEST);

	// Load and compile shader
	program = loadShaders("src/test.vert", "src/test.frag");
	printError("error loading shaders");

    // Init camera
    c = Camera(program);

    // Set Texture units
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

    // Create and upload projection matrix
    projection_matrix = frustum(left, right, bottom, top, near, far);
    glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_TRUE, projection_matrix.m);
	printError("error loading projection");

    //glUniform1i(glGetUniformLocation(program, "spacebox"), 0);
}

void display(void)
{
	printError("pre display");

	// clear the screen
    s.draw(program);
    b.draw(program);
    d.draw(program);
    p.draw(program);
    e.draw(program);
    f.draw(program);
    //c.rotate('y', 0.01);
	printError("draw error");

	SDL_GL_SwapBuffers();
}

Uint32 OnTimer(Uint32 interval, void* param)
{
	a += 0.1;

    // För att få bort varningar
    param = NULL;
    param = param;

    b.update(interval/1000.0);
    sys.bodies.update(interval/1000.0);

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
	init();
	SDL_TimerID timer_id;
	timer_id = SDL_AddTimer(20, &OnTimer, NULL);
	if(timer_id == NULL){
		fprintf(stderr, "Error setting timer function: %s", SDL_GetError());
	}

	inf_loop();
}
