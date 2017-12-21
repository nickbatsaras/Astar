CC = gcc
FLAG = 
CFLAGS = -g -Wall

.PHONY: all clean

all: astar

astar: astar.c
	$(CC) $(CFLAGS) $(FLAG) -o $@ $^

clean:
	@rm astar 2> /dev/null || true
