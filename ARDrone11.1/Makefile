#Root/Makefile

CC=g++
LDFLAGS = `sdl-config --libs`
CCFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv`

OBJ = Model/*.o Main/*.o Controller/*.o View/*.o

all:
	make -C Model
	make -C Main
	make -C Controller
	make -C View
	$(CC) $(OBJ) -o ARDrone $(LDFLAGS) $(CCFLAGS)

clean:
	make -C Model clean
	make -C Main clean
	make -C Controller clean
	make -C View clean
	rm -rf *.o
