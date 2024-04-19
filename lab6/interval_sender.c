//
// Created by Michał Kawa on 19/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH "interval"

int main(int argc, char *argv[]) {

    mkfifo(FIFO_PATH, 0666);

    if (argc != 3) {
        printf("Usage: %s <start> <end>\n", argv[0]);
        return 1;
    }

    double start = atof(argv[1]);
    double end = atof(argv[2]);

    int fd = open(FIFO_PATH, O_WRONLY);

    char buffer[255];
    sprintf(buffer, "%f %f", start, end);
    write(fd, buffer, strlen(buffer) + 1);
    close(fd);

    fd = open(FIFO_PATH, O_RDONLY);

    double result;
    read(fd, &result, sizeof(double));
    printf("Integral value: %f\n", result);

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}
