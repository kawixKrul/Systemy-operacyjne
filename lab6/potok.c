#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int main() {
/*
    int fd[2];
    int fdA[2];
    pipe(fd);
    pipe(fdA);
    pid_t pid = fork();
    if (pid == 0) {
        //close(fd[1]);
        char buffer[256];
        read(fd[0], buffer, sizeof(buffer));
        printf("Received child: %s\n", buffer);
        strcat(buffer, " siwy dym\0");
        close(fdA[0]);
        //write(fdA[1], buffer, sizeof(char)*strlen(buffer));
        write(fd[1], buffer, sizeof(char)*strlen(buffer));
    } else {
        //close(fd[0]);
        char *msg = "zadymka fiu fiu";
        write(fd[1], msg, sizeof(char)*strlen(msg));
        close(fdA[1]);
        char buffer[256];
        //read(fdA[0], buffer, sizeof(buffer));
        read(fd[0], buffer, sizeof(buffer));
        printf("Received parent: %s\n", buffer);
    }
*/

    /*FILE *wc_input = popen("wc -l", "w");
    FILE *ps_output = popen("ps aux", "r");

    char buffer[1024];
    while(fread(buffer, sizeof(char), 1024, ps_output) > 0) {
        fputs(buffer, wc_input);
    }

    pclose(wc_input);
    pclose(ps_output);*/

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
    } else {
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("ps","ps" ,"aux", NULL);
    }

    return 0;
}