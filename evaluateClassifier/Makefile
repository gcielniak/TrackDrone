CXX=g++
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = evaluateClassifier.cpp

all:
	$(CC) $(OBJ) -o evaluateClassifier $(CCFLAGS)

clean:
	rm -rf *.o
