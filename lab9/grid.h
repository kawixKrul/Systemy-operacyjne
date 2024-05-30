#pragma once
#include <stdbool.h>
#include <pthread.h>

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void *update_grid(void *arg);

typedef struct {
    int id;
    int start;
    int end;
    char *foreground;
    char *background;
    pthread_t thread;
    sigset_t *signal_set;
} thread_data_t;