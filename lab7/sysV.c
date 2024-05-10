//
// Created by Michał Kawa on 26/04/2024.
//
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
    long mtype;         /* typ komunikatu   */
    char mtext[256];      /* tresc komunikatu */
};


int main() {
    key_t key = ftok(".", 'c');
    int queue = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf msg;
    msg.mtype = 1;
    strcpy(msg.mtext, "siema eniu");
    msgsnd(queue, &msg, sizeof(msg), MSG_NOERROR);

    msg.mtype = 2;
    strcpy(msg.mtext, "zegnaj swiecie");
    msgsnd(queue, &msg, sizeof(msg), MSG_NOERROR);

    struct msqid_ds q_buf;
    struct msgbuf buffer;
    for (int i = 0; i < 2; ++i) {
        msgctl(queue, IPC_STAT, &q_buf);
        printf("liczba komunikatow: %lu\nPID obdbierajacego: %d\n", q_buf.msg_qnum, q_buf.msg_lrpid);
        msgrcv(queue, &buffer, sizeof(msg), 0, MSG_NOERROR);
        printf("%s\n", buffer.mtext);
    }
    //printf("%s", buffer.mtext);
    return 0;
}