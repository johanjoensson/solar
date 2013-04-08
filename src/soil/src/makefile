LIB = libSOIL.a

LIBDIR = ../lib

CXX = g++
CXXFLAGS = -o2 -s -Wall

SRCNAMES = 	image_helper.c \
		stb_image_aug.c \
		image_DXT.c \
		SOIL.c \

OBJ = $(SRCNAMES:.c=.o)
BIN = $(LIBDIR)/$(LIB)

all: $(BIN)

$(BIN): $(OBJ)
	ar r $(BIN) $(OBJ)
	ranlib $(BIN)

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm $(OBJ)
