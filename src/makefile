CC = gcc
CXX = g++

LD_LIBRARY_PATH = soil/lib
INC = -Isoil/src/
WARN = -Wall -Wextra -W -pedantic 
ETC = -DGL_GLEXT_PROTOTYPES -g 
LIB = -lGL  -lm `sdl-config --cflags --libs`

CFLAGS = $(INC) $(WARN) $(ETC) $(LIB) -std=c99

CXFLAGS = $(INC) $(WARN) $(ETC) $(LIB) -std=c++0x

EXE = test

CXSRC = test.cpp \
	object.cpp \
	camera.cpp \
	spacebox.cpp

CSRC = SDL_util.c \
       VectorUtils3.c \
       GL_utilities.c \
       LoadTGA.c

SRC = $(CXSRC) $(CSRC)

OBJ = $(patsubst %.c, %.o, $(CSRC)) $(patsubst %.cpp, %.o, $(CXSRC))


all : $(EXE)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o : %.cpp
	$(CXX) $(CXFLAGS) -c $^ -o $@

test: test.o SDL_util.o VectorUtils3.o object.o body.o GL_utilities.o loadobj.o LoadTGA.o camera.o spacebox.o system.o soil/lib/libSOIL.a
	$(CXX) $(CXFLAGS) $^ -o test

clean:
	rm *.o
	rm $(EXE)
