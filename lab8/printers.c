#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define N 5 // liczba użytkowników
#define M 3 // liczba drukarek
#define QUEUE_SIZE 10
#define FTOK_PATH "/tmp"

typedef struct {
    char jobs[QUEUE_SIZE][11];
    int front;
    int rear;
    int count;
} PrintQueue;


int shm_id;
int sem_id;
PrintQueue* queue;
pid_t user_pids[N];
pid_t printer_pids[M];

void sem_op(int sem_id, int sem_num, int op) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = op;
    sb.sem_flg = 0;
    semop(sem_id, &sb, 1);
}

void enqueue(const char* job) {
    strcpy(queue->jobs[queue->rear], job);
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->count++;
}

void dequeue(char* job) {
    strcpy(job, queue->jobs[queue->front]);
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->count--;
}

void user_process(int id) {
    char job[11];

    srand(time(NULL) ^ (getpid()<<16));
    while (1) {
        for (int i = 0; i < 10; i++) {
            job[i] = 'a' + rand() % 26;
        }
        job[10] = '\0';

        sem_op(sem_id, 1, -1); // wait for empty slot
        sem_op(sem_id, 0, -1); // lock queue

        enqueue(job);
        printf("User %d added job: %s\n", id, job);

        sem_op(sem_id, 0, 1); // unlock queue
        sem_op(sem_id, 2, 1); // signal full slot

        sleep(rand() % 5 + 7);
    }
}

void printer_process(int id) {
    char job[11];

    while (1) {
        sem_op(sem_id, 2, -1); // wait for full slot
        sem_op(sem_id, 0, -1); // lock queue

        dequeue(job);
        printf("Printer %d printing job: %s\n", id, job);

        sem_op(sem_id, 0, 1); // unlock queue
        sem_op(sem_id, 1, 1); // signal empty slot

        for (int i = 0; i < 10; i++) {
            printf("%c", job[i]);
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
    }
}

void cleanup(int signum) {
    printf("Cleaning up resources...\n");

    for (int i = 0; i < N; i++) {
        kill(user_pids[i], SIGTERM);
    }

    for (int i = 0; i < M; i++) {
        kill(printer_pids[i], SIGTERM);
    }

    for (int i = 0; i < N + M; i++) {
        wait(NULL);
    }

    shmdt(queue);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, 0);

    exit(EXIT_SUCCESS);
}

int main() {
    key_t queue_key = ftok(getenv(FTOK_PATH), 'Q');
    key_t sem_key = ftok(getenv(FTOK_PATH), 'S');

    signal(SIGINT, cleanup);

    shm_id = shmget(queue_key, sizeof(PrintQueue), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    queue = (PrintQueue*)shmat(shm_id, NULL, 0);
    if (queue == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    sem_id = semget(sem_key, 3, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    union semun sem_union;
    sem_union.val = 1;
    semctl(sem_id, 0, SETVAL, sem_union);

    sem_union.val = QUEUE_SIZE;
    semctl(sem_id, 1, SETVAL, sem_union);

    sem_union.val = 0;
    semctl(sem_id, 2, SETVAL, sem_union);

    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            user_process(i + 1);
            exit(EXIT_SUCCESS);
        } else {
            user_pids[i] = pid;
        }
    }

    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printer_process(i + 1);
            exit(EXIT_SUCCESS);
        } else {
            printer_pids[i] = pid;
        }
    }

    for (int i = 0; i < N + M; i++) {
        wait(NULL);
    }

    cleanup(0);

    return 0;
}
