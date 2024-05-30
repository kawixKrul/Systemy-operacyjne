CC = gcc
CFLAGS = -Wall -I/opt/homebrew/opt/ncurses/include
LDFLAGS = -L/opt/homebrew/opt/ncurses/lib

all: life lab
	
life: grid.c life.c
	$(CC) $(CFLAGS) $(LDFLAGS) grid.c life.c -o life -lncursesw

lab: lab.o
	$(CC) $(CFLAGS) lab.o -o lab

lab.o: lab.c
	$(CC) $(CFLAGS) -c lab.c -o $@

clean:
	rm -f life *.o *.exe *.dSYM lab
