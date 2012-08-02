#Model/Makefile

CXX=g++

LDFLAGS = `sdl-config --libs`
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = ArDrone.o Communication.o Command.o Video.o NavigationData.o

all: $(OBJ)

%.o : %.cpp
	$(CXX) -c $^ -o $@ $(LDFLAGS) $(CCFLAGS)

clean:
	rm -rf *.o
