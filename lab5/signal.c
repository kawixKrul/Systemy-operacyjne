#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void handler(int signum) {
    printf("Received SIGUSR1 signal\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: at least 1 argument required");
        return 1;
    }

    if (strcmp(argv[1], "none") == 0) {

    } else if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
    } else if (strcmp(argv[1], "mask") == 0) {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, handler);
    } else {
        printf("ERROR: Bad argument");
        return 1;
    }

    raise(SIGUSR1);

    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGUSR1)) {
        printf("Pending SIGUSR1 \n");
    } else {
        printf("No pending SIGUSR1\n");
    }
    return 0;
}
