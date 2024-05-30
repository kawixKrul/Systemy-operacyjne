#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

void *reindeer(void *n) {
    sleep((int)rand()%10)
}

int main() {
    return 0;
}