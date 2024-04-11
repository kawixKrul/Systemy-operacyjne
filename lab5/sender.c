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

void handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Received confirmation from catcher\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <catcher_pid> <work_mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t catcher_pid = atoi(argv[1]);
    int work_mode = atoi(argv[2]);

    if (work_mode < 1 || work_mode > MAX_WORK_MODE) {
        fprintf(stderr, "Invalid work mode\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    union sigval value;
    value.sival_int = work_mode;
    if (sigqueue(catcher_pid, SIGUSR1, value) == -1) {
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    while (1) {
        sigsuspend(&mask);
        break;
    }

    return 0;
}
