#View/Makefile

CXX=g++

LDFLAGS = `sdl-config --libs`
CCFLAGS = 

OBJ = Display.o EventKeyboard.o

all: $(OBJ)

%.o : %.cpp
	$(CXX) -c $^ -o $@ $(LDFLAGS) $(CCFLAGS)

clean:
	rm -rf *.o
