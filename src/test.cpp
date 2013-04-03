#define MAINFILE

#include <math.h>
#include "GL_utilities.h"
#include "SDL_util.h"
#include "body.h"

// Globals
GLfloat a = 0.0;

// Reference to shader program
GLuint program;


// Data would normally be read from files
GLfloat vertices[8][3] = {
            {-0.5f,-0.5f,0.5f},
			{-0.5f,0.5f,0.5f},
			{-0.5f,0.5f,-0.5f},
			{-0.5f,-0.5f,-0.5f},
			//
			{0.5f,-0.5f,0.5f},
			{0.5f,0.5f,0.5f},
			{0.5f,0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f} };

GLfloat colors[8][3] = {
            {0.3f,0.0f,0.1f},
			{0.5f,0.2f,0.5f},
			{0.1f,0.9f,0.5f},
			{0.8f,0.4f,0.5f},
			//
			{0.5f,0.5f,0.5f},
			{0.1f,0.8f,0.4f},
			{0.8f,0.1f,0.7f},
			{0.0f,0.7f,0.2f} };

GLubyte indices[] = {
            0,1,5, 0,4,5,	// Fram
			5,4,7, 5,6,7,	// Höger
			6,7,3, 6,3,2,	// Bak
			2,6,1, 6,1,5,	// Upp
			3,2,0, 2,0,1,	// Vänster
			0,3,7, 0,7,4};	// Ner

GLfloat matrix1[] = { 	
            1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix2[] = { 	
            1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

GLfloat matrix3[] = { 	
            1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };


// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// two vertex buffer objects, used for uploading the
	unsigned int vertexBufferObjID;
	unsigned int indexBufferObjID;
	unsigned int colorBufferObjID;
	
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.3,0.2,1.0);
	
	glEnable(GL_DEPTH_TEST);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	printError("Upload vertices");

	// End of upload of geometry

	// Upload colours
	
	glGenBuffers(1, &colorBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Colour"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Colour"));
	
	printError("Upload colours");
	
	// Upload indices
	
	glGenBuffers(1, &indexBufferObjID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*3*sizeof(GLuint), indices, GL_STATIC_DRAW);
	
	printError("Upload indices");
	
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

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);	// draw object
	
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
