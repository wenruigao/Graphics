BIN_FILE = a.exe
CFLAGS = -Wall -O2

$(BIN_FILE): lodepng.o a.o
	g++ $(CFLAGS) -o $(BIN_FILE) lodepng.o a.o

lodepng.o: lodepng.c lodepng.h
	g++ $(CFLAGS) -c lodepng.c -o lodepng.o

a.o: main.cc *.h
	g++ $(CFLAGS) -c main.cc -o a.o