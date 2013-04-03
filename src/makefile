CC = gcc
CXX = g++

WARN = -Wall -Wextra -W -pedantic 
ETC = -DGL_GLEXT_PROTOTYPES -g 
LIB = -lGL  -lm `sdl-config --cflags --libs` 

CFLAGS = $(WARN) $(ETC) $(LIB) -std=c99

CXFLAGS = $(WARN) $(ETC) $(LIB)

EXE = test

all : $(EXE)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cpp
	$(CXX) $(CXFLAGS) -c $^ -o $@

test: test.o SDL_util.c VectorUtils3.o object.o body.o GL_utilities.c loadobj.o
	$(CXX) $(CXFLAGS) $^ -o test

clean:
	rm *.o
	rm $(EXE)
