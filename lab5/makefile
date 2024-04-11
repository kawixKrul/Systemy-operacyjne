CFLAGS = -Wall -O0 -std=c17
DEBUG_FLAGS = -g -std=c17 -Wall -O0
CC = clang

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