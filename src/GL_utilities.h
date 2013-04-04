#ifndef _GL_UTILITIES_
#define _GL_UTILITIES_

// #include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Missing vitals:
// Mouse
// Key up
// Key still down function
// Menu?
// Sphere, teapot, torus? Bunny?
// New much needed additions not yet in official GLUT:
//      glutInitContextVersion (3, 1);
//      glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

// Textures?
// Models?

// Additions
#ifdef __cplusplus
extern "C" {
#endif
void printError(const char *functionName);
GLuint loadShaders(const char *vertFileName, const char *fragFileName);
void dumpInfo(void);
#ifdef __cplusplus
}
#endif
#endif
