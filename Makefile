CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=-lIL -lpng
SOURCES=color.cpp image.cpp puzzle.cpp make_pieces.cpp join_pieces.cpp match_pieces.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: make_pieces join_pieces match_pieces

make_pieces: color.o image.o puzzle.o make_pieces.o
	$(CC) color.o image.o puzzle.o make_pieces.o $(LDFLAGS) -o make_pieces

match_pieces: color.o image.o puzzle.o match_pieces.o
	$(CC) color.o image.o puzzle.o match_pieces.o $(LDFLAGS) -o match_pieces

join_pieces: color.o image.o puzzle.o join_pieces.o
	$(CC) color.o image.o puzzle.o join_pieces.o $(LDFLAGS) -o join_pieces

clean:
	rm -f make_pieces match_pieces join_pieces $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
