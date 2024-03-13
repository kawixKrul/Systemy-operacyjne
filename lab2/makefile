CFLAGS = -Wall -O0 -g -std=c17
CC = gcc

files = main

.PHONY: clean all shared static dynamic

all: shared static dynamic

static: main.c libcollatz.a
	$(CC) $(CFLAGS) main.c -o static -L. libcollatz.a

shared: main.c libcollatz.so
	$(CC) $(CFLAGS) main.c -o shared -L. libcollatz.so

dynamic: main_dyn.o libcollatz.so
	$(CC) $(CFLAGS) main_dyn.o -ldl -o dynamic

libcollatz.so: collatz.o
	$(CC) $(CFLAGS) -shared -o libcollatz.so collatz.o

libcollatz.a: collatz.o
	ar rc libcollatz.a collatz.o

collatz.o: collatz.c collatz.h
	$(CC) $(CFLAGS) -c collatz.c

main_dyn.o: main.c collatz.h
	$(CC) $(CFlAGS) -D DYNAMIC -c main.c -o main_dyn.o

clean:
	rm -rf *.o *.out *.so *.a static shared dynamic *.dSYM