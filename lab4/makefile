CFLAGS = -Wall -O0 -std=c17
DEBUG_FLAGS = -g -std=c17 -Wall -O0
CC = gcc

all: cp cp_dir

hello: hello.c
	$(CC) $(CFLAGS) -c hello.c
	$(CC) $(CFLAGS) hello.o -o $@

cp: cp.c
	$(CC) $(CFLAGS) -c cp.c
	$(CC) $(CFLAGS) cp.c -o $@

cp_dir: cp_dir.c
	$(CC) $(CFLAGS) -c cp_dir.c
	$(CC) $(CFLAGS) cp_dir.c -o $@


clean:
	rm -rf *.o *.exe *.dSYM wcdir hello cp cp_dir