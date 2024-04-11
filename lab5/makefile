CFLAGS = -Wall -O0
DEBUG_FLAGS = -g  -Wall -O0
CC = gcc

all: signal catcher sender

signal: signal.c
	$(CC) $(CFLAGS) -c signal.c
	$(CC) $(CFLAGS) signal.o -o $@

catcher: catcher.c
	$(CC) $(CFLAGS) -c catcher.c
	$(CC) $(CFLAGS) catcher.o -o $@

sender: sender.c
	$(CC) $(CFLAGS) -c sender.c
	$(CC) $(CFLAGS) sender.o -o $@

clean:
	rm -rf *.o *.exe *.dSYM signal signal_reaction catcher sender
