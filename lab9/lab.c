#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define N 10

typedef struct {
    int id;
    int count;
} Counter;

void handler(int signo) {
    printf("Tu watek: %d", pthread_self());
}

void *hello(void *n) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    Counter *arg = (Counter*)n;
    arg->count = 0;
    for(;;) {
        printf("Hjelo tu watek: %d, iteracja: %d + tid: %d\n", arg->id, arg->count, pthread_self());
        sleep(1);
        arg->count++;
    }
}

void *hello_sync(void *n) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    Counter *arg = (Counter*)n;
    arg->count = 0;
    printf("Hjelo tu watek: %d, iteracja: %d + tid: %d\n", arg->id, arg->count, pthread_self());
    sleep(3);
    arg->count++;
    printf("Nie ma mnie");
}

void *value(void*n) {
    int *res = malloc(sizeof(int));
    *res = 10 + *(int*)n;
    exit(res);
}

void *signal_thread(void*) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    for(;;) {}
}

int main() {
    pthread_t threads[N];
    Counter args[N];
    int argus[N];
    signal(SIGUSR1, handler);
    for (int i = 0; i < N; ++i) {
        args[i].id = i;
        argus[i] = i;
        pthread_create(&threads[i], NULL, value, &argus[i]);
    }
    pthread_kill(threads[5], SIGUSR1);
    sleep(3);
    /*void *values[N];
    for(int i = 0; i < N; ++i) {
        pthread_join(threads[i], values[i]);
    }
    for(int i = 0; i < N; ++i) {
        printf("i: %d val: %d\n", i, *(int*)values[i]);
    }*/
    /*int flag = 1;
    while(flag) {
        for (int i = 0; i < N; ++i) {
            if (args[i].count > 5) {
                flag = 0;
            }
        }
    }
    for(int i = 0; i < N; ++i) {
        pthread_cancel(threads[i]);
    }
    printf("koniec");*/
    for(int i = 0; i < N; ++i) {
        pthread_cancel(threads[i]);
    }
    return 0;
}