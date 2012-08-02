CXX=g++
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = createListsClassifier.cpp

all:
	$(CC) $(OBJ) -o createListsClassifier $(CCFLAGS)

clean:
	rm -rf *.o
