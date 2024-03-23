//
// Created by Michał Kawa on 22/03/2024.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("ERROR: at least 1 argument required");
        return 1;
    }

    const char *dir = argv[1];

    printf("program name is: %s\n",argv[0]);
    int local = 0;

    pid_t pid = fork();
    if (pid == 0) {
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", (int)getpid(), (int)getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        int err = execl("/bin/ls", "ls", dir ,NULL);
        exit(err);
    } else {
        int i;
        waitpid(pid, &i, 0);
        int status = WEXITSTATUS(i);
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", (int)getpid(), (int)pid);
        printf("Child exit code: %d\n", status);
        printf("Parent's local = %d, parent's global = %d\n", local, global);
    }

    return 0;
}