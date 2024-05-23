#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MEM_SIZE 1024
#define SEM_NUM 1

int main() {
    /*key_t key = ftok(getenv("."), 1);
    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);

    char *addr = (char*)shmat(shmid, NULL, 0);
    strcpy(addr, "Siema");
    shmdt(addr);
    addr = (char*)shmat(shmid, NULL, 0);
    printf("%s", addr);
    shmdt(addr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;*/

    /*int fd = shm_open("essa", O_CREAT | O_RDWR, 0);
    ftruncate(fd, MEM_SIZE);
    char *addr = (char*) mmap(NULL, MEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    strcpy(addr, "Siema");
    munmap(addr, MEM_SIZE);
    char *addr2 = (char*) mmap(NULL, MEM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    printf("%s", addr2);
    munmap(addr2, MEM_SIZE);
    shm_unlink("essa");
    return 0;*/

    key_t key = ftok(getenv("."), 1);
    int semid = semget(key, SEM_NUM, 0666 | IPC_CREAT);
    union semun arg = {.val = 10};
    semctl(semid, 0, SETVAL, arg);
    struct sembuf sem_op = {
            .sem_num = 0,
            .sem_op = -1,
            .sem_flg = 0,
    };
    semop(semid, &sem_op, 1);
    int val = semctl(semid, 0, GETVAL, NULL);
    printf("semaphore value: %d\n", val);

    sem_op.sem_op = 1;
    semop(semid, &sem_op, 1);
    val = semctl(semid, 0, GETVAL, NULL);
    printf("semaphore value: %d\n", val);

    semctl(semid, NULL, IPC_RMID, NULL);
    return 0;
}