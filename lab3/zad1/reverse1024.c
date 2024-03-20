//
// Created by Michał Kawa on 19/03/2024.
//
#include "variants.h"

#ifndef VARIANTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
#endif

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("ERROR: Need exactly 2 arguments");
        return 1;
    }
    char *src = argv[1];
    char *out = argv[2];
    FILE *source = fopen(src, "rb");
    if (source == NULL) {
        printf("ERROR: cant open src file");
        return 1;
    }

    FILE *reverse = fopen(out, "wb");
    if (reverse == NULL) {
        printf("ERROR: cant open output file");
        return 1;
    }

    time_it(variant_two, source, reverse, "Time for variant 2: ");

    fclose(source);
    fclose(reverse);
}