CFLAGS = -Wall -O0
DEBUG_FLAGS = -g  -Wall -O0
CC = clang

all: printers

lab: lab.o
	$(CC) lab.o -o lab

lab.o: lab.c
	$(CC) -c lab.c -o $@ $(CFLAGS)

printers: printers.o
	$(CC) printers.o -o printers

printers.o: printers.c
	$(CC) -c printers.c -o $@ $(CFLAGS)

clean:
	rm -rf *.o *.exe *.dSYM lab printers