//
// Created by Michał Kawa on 11/04/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_WORK_MODE 3

pid_t catcher_pid;
int work_mode;
int success = 1;


void handler(int sig) {
    success = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <catcher_pid> <work_mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    signal(SIGUSR1, handler);

    catcher_pid = atoi(argv[1]);
    work_mode = atoi(argv[2]);

    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);


    union sigval value;
    value.sival_int = work_mode;
    sigqueue(catcher_pid, SIGUSR1, value);


    while (success);

    printf("Confirmation received");

    return 0;
}
