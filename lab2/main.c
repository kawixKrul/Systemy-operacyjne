#include <stdio.h>

#ifndef DYNAMIC
    #include "collatz.h"
#endif

#ifdef DYNAMIC
    #include <dlfcn.h>
    int (*test_collatz_convergence)(int, int);
#endif

#define MAX_ITER 100

int main() {

#ifdef DYNAMIC
    void *dyn_lib = dlopen("./libcollatz.so", RTLD_LAZY);
    if (!dyn_lib) {
        fprintf(stderr, "Error %s", dlerror());
        return 1;
    }
    test_collatz_convergence = dlsym(dyn_lib, "test_collatz_convergencemkdir ");
#endif

    for (int i=0; i<5; ++i) {
        printf("Podaj liczbe do konwergencji Collatza\n");
        int test;
        scanf("%d", &test);
        int result = test_collatz_convergence(test, MAX_ITER);
        if (result != -1) {
            printf("Zbiega sie po %d probach\n", result);
        } else {
            printf("Nie zbiega sie w %d probach\n", MAX_ITER);
        }
    }
    return 0;
}