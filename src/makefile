CC = g++

WARN = -Wall -Wextra -W -pedantic

ETC = -DGL_GLEXT_PROTOTYPES -g
LIB = -lGL -lGLU -lm `sdl-config --cflags --libs`

CFLAGS = $(WARN) $(ETC) $(LIB)

EXE = test

all : $(EXE)

test: test.cpp SDL_util.c VectorUtils3.c object.cpp body.cpp GL_utilities.c
	$(CC) $(CFLAGS) -o test $^

clean:
	rm $(EXE)
