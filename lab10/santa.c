#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define REINDEER_MAX 9
#define DELIVERIES_MAX 4
#define TRUE 1
#define FALSE 0

pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int reindeer_count = 0;
int deliveries_count = 0;
int finished = FALSE;

void *reindeer(void *n) {
    int id = *(int*)n;
    while(TRUE) {
        if(finished) {
            break;
        }
        sleep((rand()%6)+5);
        pthread_mutex_lock(&mutex);
        reindeer_count++;
        printf("Renifer: %d    Czeka %d reniferow\n", id, reindeer_count);
        if(reindeer_count == REINDEER_MAX) {
            printf("Renifer: %d budzi Mikolaja\n", id);
            pthread_cond_signal(&santa_cond);
        }
        while(reindeer_count > 0 && !finished) {
            pthread_cond_wait(&reindeer_cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *santa(void *n) {
    while(TRUE) {
        pthread_mutex_lock(&mutex);
        while(reindeer_count < REINDEER_MAX && deliveries_count < DELIVERIES_MAX) {
            pthread_cond_wait(&santa_cond, &mutex);
        }
        if(deliveries_count >= DELIVERIES_MAX) {
            finished = TRUE;
            pthread_cond_broadcast(&reindeer_cond);
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Mikolaj budzi sie\n");
        printf("Mikolaj dostarzca prezenty\n");
        sleep((rand()%3)+2);
        deliveries_count++;
        reindeer_count = 0;
        printf("Mikolaj zasypia\n");
        pthread_cond_broadcast(&reindeer_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t santa_thread;
    pthread_t reindeer_threads[REINDEER_MAX];
    srand(time(NULL));
    pthread_create(&santa_thread, NULL, santa, NULL);
    for (int i = 0; i < REINDEER_MAX; ++i) {
        int id = i+1;
        pthread_create(&reindeer_threads[i], NULL, reindeer, &id);
    }
    pthread_join(santa_thread, NULL);
    for (int i = 0; i < REINDEER_MAX; ++i) {
        pthread_join(reindeer_threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&santa_cond);
    pthread_cond_destroy(&reindeer_cond);
    return 0;
}