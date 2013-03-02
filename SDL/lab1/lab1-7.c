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
#include "loadobj.h"

#include "SDL_util.h"

// Globals
GLfloat a = 0.0;

// Reference to shader program
GLuint program;

Model *m;


// Data would normally be read from files
GLfloat vertices[8][3] = {{-0.5f,-0.5f,0.5f},
			{-0.5f,0.5f,0.5f},
			{-0.5f,0.5f,-0.5f},
			{-0.5f,-0.5f,-0.5f},
			//
			{0.5f,-0.5f,0.5f},
			{0.5f,0.5f,0.5f},
			{0.5f,0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f} };

GLfloat colors[8][3] = {{0.3f,0.0f,0.1f},
			{0.5f,0.2f,0.5f},
			{0.1f,0.9f,0.5f},
			{0.8f,0.4f,0.5f},
			//
			{0.5f,0.5f,0.5f},
			{0.1f,0.8f,0.4f},
			{0.8f,0.1f,0.7f},
			{0.0f,0.7f,0.2f} };

GLubyte indices[] = {	0,1,5, 0,4,5,	// Fram
			5,4,7, 5,6,7,	// Höger
			6,7,3, 6,3,2,	// Bak
			2,6,1, 6,1,5,	// Upp
			3,2,0, 2,0,1,	// Vänster
			0,3,7, 0,7,4};	// Ner

GLfloat matrix1[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix2[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix3[] = { 	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };


// vertex array object
unsigned int bunnyVertexArrayObjID;




void init(void)
{
	m = LoadModel("bunny.obj");
	if(m == NULL){
		fprintf(stderr, "Failed to load object\n");
	}
	// two vertex buffer objects, used for uploading the

	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.3,0.2,1.0);
	
	glEnable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-7.vert", "lab1-7.frag");
	printError("init shader");

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);	
	glGenBuffers(1, &bunnyIndexBufferObjID);	
	glGenBuffers(1, &bunnyNormalBufferObjID);	
	
	glBindVertexArray(bunnyVertexArrayObjID);

	// Vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// Normal VBO
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	// Upload geomerty to GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
	
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	matrix1[0] = cosf(a*0.2);
 	matrix1[1] = -sinf(a*0.2);
 	matrix1[4] = sinf(a*0.2);
 	matrix1[5] = cosf(a*0.2);

 	matrix2[5] = cosf(a*0.2);
 	matrix2[6] = -sinf(a*0.2);
 	matrix2[9] = sinf(a*0.2);
 	matrix2[10] = cosf(a*0.2);

	matrix3[0] = cosf(a*0.2);
	matrix3[2] = sinf(a*0.2);
	matrix3[8] = -sinf(a*0.2);
	matrix3[10] = cosf(a*0.2);

	glUniformMatrix4fv(glGetUniformLocation(program, "in_Matrix1"), 1, GL_TRUE, matrix1);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_Matrix2"), 1, GL_TRUE, matrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "in_Matrix3"), 1, GL_TRUE, matrix3);

	glBindVertexArray(bunnyVertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);	// draw object
	
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
