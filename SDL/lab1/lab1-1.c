// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Includes vary a bit with platforms.
// MS Windows needs GLEW or glee.
// Mac uses slightly different paths.

#include <SDL.h>

#include "GL_utilities.h"

// Globals

SDL_Surface* screen;

// Data would normally be read from files
GLfloat vertices[] = {	-0.7f,-0.5f,0.0f,
			-0.5f,0.5f,0.0f,
			0.5f,-0.8f,0.0f };
GLfloat colors[] = { 	1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f };

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;
	// Reference to shader program
	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.3,0.2,1.0);
	
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1, -1);
	
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-1.vert", "lab1-1.frag");
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
	
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors)*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_color"));

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	printError("display");
	
	SDL_GL_SwapBuffers();
}

void inf_loop()
{
	SDL_Event event;
	while(1){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						SDL_Quit();
						exit(0);
						break;
					}
					break;
				case SDL_QUIT:
						SDL_Quit();
						exit(0);
						break;
				default:
						break;
			}
		}
		display();
	}

		
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640, 480, 0, SDL_OPENGL | SDL_HWSURFACE);
	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	init ();
	SDL_GetError();
	inf_loop();
}
