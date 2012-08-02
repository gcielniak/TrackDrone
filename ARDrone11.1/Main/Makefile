#Main/Makefile

CXX=g++
LDFLAGS = `sdl-config --libs`
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = Main.o

all: $(OBJ)

%.o : %.cpp Connection.h Command.h Video.h EventKeyboard.h Display.h Controller.h NavigationData.h
	$(CXX) -c $^ -o $@ $(LDFLAGS)  $(CCFLAGS)

clean:
	rm -rf *.o
