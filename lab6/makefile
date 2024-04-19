CFLAGS = -Wall -O0
DEBUG_FLAGS = -g  -Wall -O0
CC = clang

all: potok interval interval_catcher interval_sender

potok: potok.o
	$(CC) potok.o -o potok

potok.o: potok.c
	$(CC) -c potok.c -o $@ $(CFLAGS)

interval: interval.o
	$(CC) interval.o -o interval

interval.o: interval.c
	$(CC) -c interval.c -o $@ $(CFLAGS)

interval_sender: interval_sender.o
	$(CC) interval_sender.o -o interval_sender

interval_sender.o: interval_sender.c
	$(CC) -c interval_sender.c -o $@ $(CFLAGS)

interval_catcher: interval_catcher.o
	$(CC) interval_catcher.o -o interval_catcher

interval_catcher.o: interval_catcher.c
	$(CC) -c interval_catcher.c -o $@ $(CFLAGS)

clean:
	rm -rf *.o *.exe *.dSYM potok interval interval_sender interval_catcher