CFLAGS = -Wall -O2 -g -std=c17
CXX = gcc

files = countdown

.PHONY: clean all countdown

all: $(files)

countdown: countdown.o
	$(CXX) countdown.o -o countdown.out

countdown.o: countdown.c
	$(CXX) -c countdown.c -o countdown.o $(CFLAGS)

clean:
	rm -rf *.o *.out