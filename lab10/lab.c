#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define N 10
#define CAP 100

int x = 0;
pthread_mutex_t mutex;
pthread_mutexattr_t attr;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *increment_normal(void *n) {
    pthread_mutex_lock(&mutex);
    x++;
    printf("thread: %d, x: %d\n", *(int*)n, x);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *increment_rec(void *n) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    x++;
    printf("thread: %d, x: %d\n", *(int*)n, x);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *increment_error(void *n) {
    int e1 = pthread_mutex_lock(&mutex);
    int e2 = pthread_mutex_lock(&mutex);
    if (e2 != 0) {
        perror("error");

    }
    printf("e1: %d, e2: %d, expected: %d\n", e1, e2, EDEADLK);
    x++;
    printf("thread: %d, x: %d\n", *(int*)n, x);
    return NULL;
}

void *increment_condition(void *n) {
    printf("thread sleep: %d\n", *(int*)n);
    pthread_mutex_lock(&mutex);
    while (x <= CAP) {
        pthread_cond_wait(&cond, &mutex);
    }
    x++;
    printf("thread awaken: %d, x: %d\n", *(int*)n, x);
    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main() {
    pthread_t threads[N];
    pthread_mutexattr_init(&attr);
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&mutex, &attr);
    int args[N];
    for (int i = 0; i < N; ++i) {
        args[i] = i;
        pthread_create(&threads[i], NULL, increment_condition, &args[i]);
    }
    sleep(2);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < CAP*100; ++i) {
        x++;
        if (x > CAP) {
            pthread_cond_broadcast(&cond);
        }
    }
    pthread_mutex_unlock(&mutex);
    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}