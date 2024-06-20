CC = gcc
CFLAGS = -Wall

all: client server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o $@

server: server.o
	$(CC) $(CFLAGS) server.o -o server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c -o $@

clean:
	rm -f life *.o *.exe *.dSYM client server
