CFLAGS = -Wall -O0 -std=c17
DEBUG_FLAGS = -g
CC = gcc

all: wcdir

wcdir: wcdir.c
	$(CC) $(CFLAGS) -c wcdir.c
	$(CC) $(CFLAGS) wcdir.o -o $@

test: wcdir
	./wcdir

clean:
	rm -rf *.o *.exe *.dSYM wcdir