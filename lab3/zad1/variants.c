//
// Created by Michał Kawa on 19/03/2024.
//
#ifndef VARIANTS_H
    #include <stdlib.h>
    #include <time.h>
    #include <stdio.h>
#endif

#define BUFFER_SIZE 1024

void variant_one(FILE *src, FILE *out) {
    char buffer[1];
    fseek(src, 0, SEEK_SET);
    while(fread(buffer, 1, 1, src)) {
        buffer[0] = ~buffer[0];
        fwrite(buffer, 1, 1, out);
    }
}

void variant_two(FILE *src, FILE *out) {
    char buffer[BUFFER_SIZE];
    size_t read;
    fseek(src, 0, SEEK_SET);
    while ((read = fread(buffer, sizeof(char), BUFFER_SIZE, src)) > 0) {
        for (size_t i = 0; i < read; ++i) {
            buffer[i] = ~buffer[i];
        }
        fwrite(buffer, sizeof(char), read, out);
    }
}

void time_it(void (*f)(FILE*, FILE*), FILE *src, FILE *out, char *msg) {
    clock_t start = clock();
    f(src, out);
    clock_t end = clock();
    float result = (float)(end-start)/CLOCKS_PER_SEC;
    FILE *res = fopen("pomiar_zad_1.txt", "a");
    if (res == NULL) {
        printf("ERROR: cant open measurment file");
        return;
    }
    fprintf(res, "%s %f\n", msg, result);
    fclose(res);
}