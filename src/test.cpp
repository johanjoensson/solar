#include <math.h>
#include "GL_utilities.h"
#include "SDL_util.h"
#include "body.h"
#include "loadobj.h"
#include "camera.h"
#include "VectorUtils3.h"
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
Body b;
Spacebox s;
Camera c;

mat4 projection_matrix;

void init(void)
{
	dumpInfo();
<<<<<<< HEAD
    //b = Body("bunnyplus.obj", "../res/grass.tga");
    s = Spacebox("../res/skybox.obj", "../res/grass.tga");
    //b.translate(0,0,-2);

    //b.spin_y = 2*3.14;
=======
    b = Body("src/bunnyplus.obj");
    p = Body("res/planet.obj");
    p.translate(2,0,-3);
    b.translate(0,0,-2);
    p.spin_z = 1;
    b.spin_y = 2*3.14;
    b.spin_x = 3.14;
    b.spin_z = 9;
>>>>>>> master

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
}

void display(void)
{
	printError("pre display");

	// clear the screen
    //b.draw(program);
    c.rotate('y', 0.01);
    s.draw(program);
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
	timer_id = SDL_AddTimer(20, &OnTimer, NULL);
	if(timer_id == NULL){
		fprintf(stderr, "Error setting timer function: %s", SDL_GetError());
	}

	inf_loop();
}
