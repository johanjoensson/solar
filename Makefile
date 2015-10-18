# Makefile för att automagiskt fixa allt som fixas bör, förutom att lägga
# till bibliotek att länka. Detta måste läggas till manuellt, jättejobbigt.

# Kompilera med kompilatorn för C++
CC = gcc
CXX = g++ 

SRC = src
BIN = bin

# Jag vill se ALLT som KAN vara fel!
# om saker kompileras med varningar är det för att man gjort FEL!
# Inga varningar när man kompilerar kod, capish?!
WARNINGS = -Wall -Wextra -W -pedantic

# Kompilera med lite olika flaggor, utöver varningarna.
# -g för debugmode, bör tas bort inför slutreleasen (inget måste dock).
# sdl-config för att sätta flaggor för SDL korrekt.
CXXFLAGS = -g -DGL_GLEXT_PROTOTYPES $(WARNINGS) `sdl2-config --cflags` -std=c++0x
CFLAGS = -g -DGL_GLEXT_PROTOTYPES $(WARNINGS) `sdl2-config --cflags` -std=c99

# De ytterligagre bibliotek vi behöver
# GL är OpenGL och m är matematik
LIB = GL SDL2_image

# Länka de bibliotek vi behöver
# sdl-config för att enkelt länka rätt bibliotek
LDXFLAGS = $(addprefix -L,$(NORA)) $(addprefix -l,$(LIB)) `sdl2-config --libs`

# Alla .cpp-filer skall kompileras!
# Filer som inte skall kompileras får inte ha filändelsen .cpp!
CXSOURCES = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/helper/*.cpp)
CSOURCES = $(wildcard $(SRC)/*.c)

# Vi vill ha objekt av alla våra .c/.cpp-filer
COBJ = $(patsubst $(SRC)/%.c,$(BIN)/%.o, $(CSOURCES))
CXOBJ = $(patsubst $(SRC)/%.cpp,$(BIN)/%.o, $(CXSOURCES))

OBJ = $(CXOBJ) $(COBJ)
# om någon är korkad ska detta inte förstöra min fina Makefile
.PHONY: all clean src bin

# Samtliga exekverbara filer vi vill skapa, bör bara finnas en!
EXE = solar\

# Om inget annat nämns så bygger vi alla exekverbara filer
all: bin src $(EXE)

# Samtliga .o-filer bör ha en passande .c/.cpp-fil att kompilera
# Om inte så är det inga problem (såvida vi inte kör en make clean)
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $? -o $@
	
$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $? -o $@

# Kompilera vår exekverbara fil med lämpliga flaggor samt bibliotek
solar: $(OBJ)

	$(CXX) $^ -o $@ $(LDXFLAGS)

src : 
	mkdir -p src

bin :
	mkdir -p bin/helper

# Rensa bort alla temporära filer som skapats
clean:
	rm -f $(OBJ)

# Ta bort allt som kompilerats!
cleanall:
	rm -f $(EXE) $(OBJ)
