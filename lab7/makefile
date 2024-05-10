CFLAGS = -Wall -O0
DEBUG_FLAGS = -g  -Wall -O0
CC = clang

all: client server

sysV: sysV.o
	$(CC) sysV.o -o sysV

sysV.o: sysV.c
	$(CC) -c sysV.c -o $@ $(CFLAGS)

client: client.o
	$(CC) client.o -o client

client.o: client.c consts.h
	$(CC) -c client.c -o $@ $(CFLAGS)

server: server.o
	$(CC) server.o -o server

server.o: server.c consts.h
	$(CC) -c server.c -o $@ $(CFLAGS)


clean:
	rm -rf *.o *.exe *.dSYM sysV client server server_key.txt