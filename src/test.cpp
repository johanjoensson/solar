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

// vertex array object
unsigned int planetVertexArrayObjID;
unsigned int planetVertexBufferObjID;
unsigned int planetIndexBufferObjID;

void init(void)
{
	dumpInfo();

    m = LoadModel("../res/planet.obj");

	// GL inits
	glClearColor(0.2,0.3,0.2,1.0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("test.vert", "test.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
    //
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &planetVertexArrayObjID);
    glGenBuffers(1, &planetVertexBufferObjID);
    glGenBuffers(1, &planetIndexBufferObjID);
	glBindVertexArray(planetVertexArrayObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, planetVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	printError("Upload vertices");

	// End of upload of geometry

	// Upload indices
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planetIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*2*sizeof(GLfloat), m->indexArray, GL_STATIC_DRAW);
	
	printError("Upload indices");
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(planetVertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_BYTE, NULL);	// draw object
	
	printError("display");
	
	SDL_GL_SwapBuffers();
}

Uint32 OnTimer(Uint32 interval, void* param)
{
	a += 0.1;

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

#undef MAINFILE
