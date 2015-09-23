#pragma once

#include "GL/gl.h"
#include <string>

GLuint LoadShader(const char *vertex_path, const char *fragment_path);
void dumpInfo();
void printError(std::string);

