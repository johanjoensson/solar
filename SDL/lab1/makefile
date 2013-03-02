CC = gcc

WARN = -Wall
STD = -std=c99

ETC = -DGL_GLEXT_PROTOTYPES -g
LIB = -lGL -lm

CFLAGS = $(WARN) $(STD) $(ETC) $(LIB)

EXE = lab1-1 lab1-2 lab1-3 lab1-5 lab1-6 lab1-7

all : $(EXE)

lab1-1: lab1-1.c GL_utilities.c 
	gcc -Wall -std=c99 -o lab1-1 -lGL `sdl-config --cflags --libs` -DGL_GLEXT_PROTOTYPES lab1-1.c GL_utilities.c 

lab1-2: lab1-2.c GL_utilities.c SDL_util.c
	$(CC) $(CFLAGS)  -o $@ `sdl-config --cflags --libs` $^

lab1-3: lab1-3.c GL_utilities.c SDL_util.c
	$(CC) $(CFLAGS)  -o $@ `sdl-config --cflags --libs` $^

lab1-5: lab1-5.c GL_utilities.c SDL_util.c
	$(CC) $(CFLAGS)  -o $@ `sdl-config --cflags --libs` $^

lab1-6: lab1-6.c GL_utilities.c SDL_util.c loadobj.c
	$(CC) $(CFLAGS)  -o $@ `sdl-config --cflags --libs` $^

lab1-7: lab1-7.c GL_utilities.c SDL_util.c loadobj.c
	$(CC) $(CFLAGS)  -o $@ `sdl-config --cflags --libs` $^

clean :
	rm lab1-{1,2,3,5,6,7}
