CC = gcc
CFLAGS = -Wall

all: life mutex
	
mutex: mutex.c
	$(CC) $(CFLAGS) mutex.o -o mutex

mutex.o: mutex.c
	$(CC) $(CFLAGS) mutex.c -o $@

lab: lab.o
	$(CC) $(CFLAGS) lab.o -o lab

lab.o: lab.c
	$(CC) $(CFLAGS) -c lab.c -o $@

clean:
	rm -f life *.o *.exe *.dSYM lab mutex
