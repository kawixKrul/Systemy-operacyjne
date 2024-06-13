CC = gcc
CFLAGS = -Wall

all: life santa
	
santa: santa.c
	$(CC) $(CFLAGS) santa.c -o santa -lpthread

lab: lab.o
	$(CC) $(CFLAGS) lab.o -o lab

lab.o: lab.c
	$(CC) $(CFLAGS) -c lab.c -o $@

clean:
	rm -f life *.o *.exe *.dSYM lab mutex santa
