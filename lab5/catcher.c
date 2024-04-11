#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define EXIT_MODE 3

void handler(int sig, siginfo_t *info, void *context) {
    if (sig == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        union sigval val = info->si_value;
        int work_mode = val.sival_int;
        sigqueue(sender_pid, SIGUSR1, value);
        if (received_work_mode == EXIT_MODE) {
            printf("Received mode 3. Exiting\n");
            exit(0);
        } else if (received_work_mode >= 1 && received_work_mode <= EXIT_MODE) {
            work_mode = received_work_mode;
        } else {
            printf("Invalid work mode: %d\n", received_work_mode);
        }
    }
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    printf("Catcher PID: %d\n", getpid());

    while (1) {
        pause();
    }

    return 0;
}
