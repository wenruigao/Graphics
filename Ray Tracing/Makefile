EXAMPLE_FILES = $(wildcard *.cc)
BIN_FILES = $(EXAMPLE_FILES:%.cc=%.exe)
CFLAGS = -Wall -O3

all: $(BIN_FILES)

%.exe: %.o lodepng.o
	g++ $(CFLAGS) -o $@ lodepng.o $<

lodepng.o: lodepng.c lodepng.h
	g++ $(CFLAGS) -c lodepng.c -o lodepng.o

%.o: %.cc *.h
	g++ $(CFLAGS) -c $< -o $@