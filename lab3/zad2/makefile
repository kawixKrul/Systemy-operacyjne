CFLAGS = -Wall -O0 -std=c17
DEBUG_FLAGS = -g -std=c17 -Wall -O0
CC = gcc

all: wcdir

wcdir: wcdir.c
	$(CC) $(CFLAGS) -c wcdir.c
	$(CC) $(CFLAGS) wcdir.o -o $@

test: wcdir
	ls -la | grep '^-' | awk '{sum += $$5} END {print "size of files:" sum}'
	./wcdir

clean:
	rm -rf *.o *.exe *.dSYM wcdir