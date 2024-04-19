//
// Created by Michał Kawa on 19/04/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <step> <processes>\n", argv[0]);
        return 1;
    }

    time_t start = clock();

    double step = atof(argv[1]);
    int processes = atoi(argv[2]);
    double result = 0;
    double interval = 1.0/processes;

    int fd[processes][2];

    for (int i = 0; i < processes; ++i) {
        pipe(fd[i]);
        pid_t child = fork();
        if (child == 0) {
            close(fd[i][0]);
            double partial = calculate_integral(i* interval, (i+1)*interval, step, f);
            write(fd[i][1], &partial, sizeof(double));
            close(fd[i][1]);
            return 0;
        }
    }

    double partial;

    for (int i = 0; i < processes; i++) {
        close(fd[i][1]);
        read(fd[i][0], &partial, sizeof(double));
        result += partial;
        close(fd[i][0]);
    }

    for (int i = 0; i < processes; i++) {
        wait(NULL);
    }

    time_t end = clock();

    printf("Integral result: %f\n", result);
    printf("Time taken: %f\n", ((double)(end-start))/CLOCKS_PER_SEC);
    return 0;
}