//
// Created by Michał Kawa on 22/03/2024.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("ERROR: at least 1 argument required");
        return 1;
    }

    int p_num = atoi(argv[1]);
    pid_t ppid;

    for (int i = 0; i < p_num; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("PID: %d, PPID: %d\n", (int)getpid(), (int)getppid());
            exit(0);
        }
    }

    while((ppid = wait(NULL)) > 0);
    printf("argv[1]: %s", argv[1]);

    return 0;
}