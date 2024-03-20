CFLAGS = -Wall -O0 -g -std=c17
CC = gcc

files = main

.PHONY: clean all reverse reverse1024

all: reverse reverse1024

reverse: reverse.c libvariants.so
	$(CC) $(CFLAGS) reverse.c -o reverse -L. -lvariants

reverse1024: reverse1024.c libvariants.so
	$(CC) $(CFLAGS) reverse1024.c -o reverse1024 -L. -lvariants

libvariants.so: variants.o
	$(CC) $(CFLAGS) -shared -o libvariants.so variants.o

variants.o: variants.c variants.h
	$(CC) $(CFLAGS) -c variants.c

test: reverse1024 reverse source.bin
	./reverse source.bin tmp_rev_1
	./reverse tmp_rev_1 tmp_rev_2
	./reverse1024 source.bin tmp_rev1024_1
	./reverse1024 tmp_rev1024_1 tmp_rev1024_2
	@echo "Wait for writing files"
	@sleep 1
	@echo "Waiting done"
	diff -s source.bin tmp_rev_2
	diff -s source.bin tmp_rev1024_2
	cat pomiar_zad_1.txt

clean:
	rm -rf *.out tm* *.a *.so *.dSYM *.o *.txt reverse reverse1024