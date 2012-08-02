CXX=g++
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = createVideo.cpp

all:
	$(CC) $(OBJ) -o createVideo $(CCFLAGS)

clean:
	rm -rf *.o
