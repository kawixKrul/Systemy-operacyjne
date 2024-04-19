//
// Created by Michał Kawa on 19/04/2024.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_PATH "interval"

double f(double x) {
    return 4/(x*x+1);
}

double calculate_integral(double start, double end, double step, double f(double)) {
    double result = 0;
    double x = start;
    while (x < end) {
        result += f(x) * step;
        x += step;
    }
    return result;
}

int main() {
    mkfifo(FIFO_PATH, 0666);

    int fd = open(FIFO_PATH, O_RDONLY);

    double start, end;
    char buffer[255];
    read(fd, buffer, sizeof(buffer));
    sscanf(buffer, "%lf %lf", &start, &end);
    close(fd);


    double result = calculate_integral(start, end, 0.01, f);

    fd = open(FIFO_PATH, O_WRONLY);

    write(fd, &result, sizeof(double));
    close(fd);

    return 0;
}