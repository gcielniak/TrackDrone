#Controller/Makefile

CXX=g++

LDFLAGS = 
CCFLAGS = 

OBJ = Controller.o

all: $(OBJ)

%.o : %.cpp
	$(CXX) -c $^ -o $@ $(LDFLAGS) $(CCFLAGS)

clean:
	rm -rf *.o
