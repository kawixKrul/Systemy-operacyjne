#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define EXIT_MODE 3

int changed_work_mode = 0;
int current_work_mode = -1;

void handler(int sig, siginfo_t *info, void *context) {
    if (sig == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        union sigval val = info->si_value;
        int work_mode = val.sival_int;
        if (work_mode != current_work_mode) {
            current_work_mode = work_mode;
            changed_work_mode++;
        }
        if (work_mode == EXIT_MODE) {
            printf("Received mode 3. Exiting\n");
            kill(sender_pid, SIGUSR1);
            exit(0);
        } else if (work_mode == 1) {
            for (int i = 1; i <= 100; ++i) {
                printf("%d\n", i);
            }
        } else if (work_mode == 2) {
            printf("Changed work mode %d times\n", changed_work_mode);
        } else {
            printf("Invalid work mode: %d\n", work_mode);
        }
        kill(sender_pid, SIGUSR1);
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
