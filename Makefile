# Makefile för att automagiskt fixa allt som fixas bör, förutom att lägga
# till bibliotek att länka. Detta måste läggas till manuellt, jättejobbigt.

# Kompilera med kompilatorn för C++
CC = g++

# Jag vill se ALLT som KAN vara fel!
# om saker kompileras med varningar är det för att man gjort FEL!
# Inga varningar när man kompilerar kod, capish?!
WARNINGS = -Wall -Wextra -W -pedantic

# Kompilera med lite olika flaggor, utöver varningarna.
# -g för debugmode, bör tas bort inför slutreleasen (inget måste dock).
# sdl-config för att sätta flaggor för SDL korrekt.
CXXFLAGS = -g $(WARNINGS) `sdl-config --cflags`

# De ytterligagre bibliotek vi behöver
# GL är OpnGL och m är matematik
$(LIB) = GL m

# Länka de bibliotek vi behöver
# sdl-config för att enkelt länka rätt bibliotek
LDFLAGS = $(addprefix -l,$(LIB)) `sdl-config --libs`

# Alla .cpp-filer skall kompileras!
# Filer som inte skall kompileras får inte ha filändelsen .cpp!
SOURCES = $(wildcard *.cpp)

# Vi vill ha objekt av alla våra .c-filer
OBJ = $(patsubst %.c, %.o, $(SOURCES))

# om någon är korkad ska detta inte förstöra min fina Makefile
.PHONY: all clean

# Samtliga exekverbara filer vi vill skapa, bör bara finnas en!
EXE = program\

# Om inget annat nämns så bygger vi alla exekverbara filer
all: $(EXE)

# Samtliga .o-filer bör ha en passande .c-fil att kompilera
# Om inte så är det inga problem (såvida vi inte kör en make clean)
%.o: %.c
	$(CC) $(CXXFLAGS) $? -o $@

# TODO: Att byta ut mot lämpligt namn på programmet!
# Kompilera vår exekverbara fil med lämpliga flaggor samt bibliotek
program: $(OBJ)
	$(CC) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Rensa bort alla temporära filer som skapats
clean:
	rm -f $(OBJ)

# Ta bort allt som kompilerats!
cleanall:
	rm -f $(EXE) $(OBJ)
