//
// Created by Michał Kawa on 26/04/2024.
//
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
    long mtype;         /* typ komunikatu   */
    char mtext[256];      /* tresc komunikatu */
};


int main() {
    mqd_t queue = mq_open()

    return 0;
}