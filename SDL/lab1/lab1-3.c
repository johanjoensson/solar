// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include <math.h>

#include "GL_utilities.h"

#include "SDL_util.h"

// Globals
GLfloat a = 0.0;

// Reference to shader program
GLuint program;


// Data would normally be read from files
GLfloat vertices[] = {	-0.7f,-0.5f,0.0f,
			-0.5f,0.5f,0.0f,
			0.5f,-0.8f,0.0f };
GLfloat colors[] = { 	0.3f, 0.4f, 0.3f,
			0.2f, 1.0f, 0.2f,
			0.7f, 0.9f, 1.0f };

GLfloat myMatrix[] = {	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

// vertex array object
unsigned int vertexArrayObjID;




void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.3,0.2,1.0);
	
	glEnable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-3.vert", "lab1-3.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// End of upload of geometry

	// Upload colours
	
	glGenBuffers(1, &colorBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Colour"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Colour"));
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myMatrix[0] = cosf(a);
	myMatrix[1] = -sinf(a);
	myMatrix[4] = sinf(a);
	myMatrix[5] = cosf(a);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "in_Matrix"), 1, GL_TRUE, myMatrix);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	printError("display");
	
	SDL_GL_SwapBuffers();
}

Uint32 OnTimer(Uint32 interval, void* param)
{
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = (int)CUSTOM_TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;

	SDL_PushEvent(&event);
	return interval;
}

int main(int argc, char *argv[])
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
