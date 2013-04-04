CC = gcc
CXX = g++

WARN = -Wall -Wextra -W -pedantic 
ETC = -DGL_GLEXT_PROTOTYPES -g 
LIB = -lGL  -lm `sdl-config --cflags --libs` 

CFLAGS = $(WARN) $(ETC) $(LIB) -std=c99

CXFLAGS = $(WARN) $(ETC) $(LIB) -std=c++11

EXE = test

CXSRC = test.cpp \
	object.cpp

CSRC =	SDL_util.c \
	VectorUtils3.c \
	GL_utilities.c

SRC = $(CXSRC) $(CSRC)

OBJ = $(patsubst %.c, %.o, $(CSRC)) $(patsubst %.cpp, %.o, $(CXSRC))


all : $(EXE)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cpp
	$(CXX) $(CXFLAGS) -c $^ -o $@

test: test.o SDL_util.o VectorUtils3.o object.o body.o GL_utilities.o loadobj.o
	$(CXX) $(CXFLAGS) $^ -o test

clean:
	rm *.o
	rm $(EXE)
