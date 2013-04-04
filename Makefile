# Makefile för att automagiskt fixa allt som fixas bör, förutom att lägga
# till bibliotek att länka. Detta måste läggas till manuellt, jättejobbigt.

# Kompilera med kompilatorn för C++
CC = gcc
CXX = g++

# Jag vill se ALLT som KAN vara fel!
# om saker kompileras med varningar är det för att man gjort FEL!
# Inga varningar när man kompilerar kod, capish?!
WARNINGS = -Wall -Wextra -W -pedantic

# Kompilera med lite olika flaggor, utöver varningarna.
# -g för debugmode, bör tas bort inför slutreleasen (inget måste dock).
# sdl-config för att sätta flaggor för SDL korrekt.
CXXFLAGS = -g $(WARNINGS) `sdl-config --cflags` -std=c++0x
CFLAGS = -g $(WARNINGS) `sdl-config --cflags` -std=c99

# De ytterligagre bibliotek vi behöver
# GL är OpnGL och m är matematik
LIB = GL m

# Länka de bibliotek vi behöver
# sdl-config för att enkelt länka rätt bibliotek
LDFLAGS = $(addprefix -l,$(LIB)) `sdl-config --libs`

# Alla .cpp-filer skall kompileras!
# Filer som inte skall kompileras får inte ha filändelsen .cpp!
CXSOURCES = $(wildcard *.cpp)

# Vi vill ha objekt av alla våra .c-filer
CXOBJ = $(patsubst %.cpp, %.o, $(CXSOURCES))

OBJ = $(CXOBJ) $(COBJ)

# om någon är korkad ska detta inte förstöra min fina Makefile
.PHONY: all clean

# Samtliga exekverbara filer vi vill skapa, bör bara finnas en!
EXE = test\

# Om inget annat nämns så bygger vi alla exekverbara filer
test: $(EXE)

# Samtliga .o-filer bör ha en passande .c/.cpp-fil att kompilera
# Om inte så är det inga problem (såvida vi inte kör en make clean)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

%.o: %.c
	$(CC) $(CFLAGS) $? -o $@
	

# TODO: Att byta ut mot lämpligt namn på programmet!
# Kompilera vår exekverbara fil med lämpliga flaggor samt bibliotek
test: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# Rensa bort alla temporära filer som skapats
clean:
	rm -f $(OBJ)

# Ta bort allt som kompilerats!
cleanall:
	rm -f $(EXE) $(OBJ)
