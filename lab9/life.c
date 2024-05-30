#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#define GRID_WIDTH 30
#define GRID_HEIGHT 30

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

    init_grid(foreground);

    if(n > GRID_HEIGHT * GRID_WIDTH) {
        n = GRID_HEIGHT * GRID_WIDTH;
    }

    pthread_t threads[n];
    thread_data_t thread_data[n];

    int thread_cells = GRID_HEIGHT * GRID_WIDTH / n;
    int remaining_cells = GRID_HEIGHT * GRID_WIDTH % n;

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    for (int i = 0; i < n; ++i) {
        thread_data[i].id = i;
        thread_data[i].start = i * thread_cells;
        thread_data[i].end = (i == n-1) ? (i * thread_cells + thread_cells + remaining_cells) : (i * thread_cells + thread_cells);
        thread_data[i].foreground = foreground;
        thread_data[i].background = background;
        thread_data[i].signal_set = &set;
        pthread_create(&threads[i], NULL, update_grid, &thread_data[i]);
    }

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
        for (int i = 0; i < n; ++i) {
            pthread_kill(threads[i], SIGUSR1);
        }

		tmp = foreground;
		foreground = background;
		background = tmp;

        for (int i = 0; i < n; ++i) {
            thread_data[i].foreground = foreground;
            thread_data[i].background = background;
        }
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
