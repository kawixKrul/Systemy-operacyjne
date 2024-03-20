//
// Created by Michał Kawa on 18/03/2024.
//

#ifndef VARIANTS_H
#define VARIANTS_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void variant_one(FILE *src, FILE *out);
void variant_two(FILE *src, FILE *out);
void time_it(void (*f)(FILE*, FILE*), FILE *src, FILE *out, char *msg);

#endif
