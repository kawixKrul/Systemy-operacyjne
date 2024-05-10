#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "consts.h"

#define MAX_CLIENTS 10

int msqid;
int clients_num = 0;
int client_msqids[MAX_CLIENTS];


void INIT_handler(msg_buf msg){
    key_t client_key = (key_t) msg.value;
    int client_msqid = msgget(client_key, 0666 | IPC_CREAT);
    msg_buf response;
    response.mtype = INIT;

    if (clients_num < MAX_CLIENTS) {
        // find id
        for (int new_client_id = 0; new_client_id < MAX_CLIENTS; new_client_id++){
            if (client_msqids[new_client_id] == -1)
                break;
        }
        if (new_client_id == MAX_CLIENTS){
            printf("Error: cannot find free client ID\n");
            return;
        }
        response.value = new_client_id;
        strcpy(response.text, "");

        // add client
        client_msqids[new_client_id] = client_msqid;
        printf("New client connected with ID: %d\n", new_client_id);
        ++clients_num;
    }
    else{
        response.value = -1;
        strcpy(response.text, "Server is full");
        printf("Server is full\n");
    }
    msgsnd(client_msqid, &response, sizeof(msg_buf) - sizeof(long), 0); // Send response
}

void MESSAGE_ALL_handler(msg_buf msg){
    for (int client_id = 0; client_id < MAX_CLIENTS; client_id++){
        if(client_msqids[client_id] != -1 && client_id != msg.sender_id){
            msgsnd(client_msqids[client_id], &msg, sizeof(msg_buf) - sizeof(long), 0);
            printf("Message from client %d sent to client %d\n", msg.sender_id, client_id);
        }
    }
}

void SIGINT_handler(){
    exit(EXIT_SUCCESS);
}

void clean_up(){
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    printf("Server message queue closed successfully\n");
}


int main() {
    atexit(clean_up);
    signal(SIGINT, SIGINT_handler);

    // initialize clients array
    for (int i=0; i<MAX_CLIENTS; i++)
        client_msqids[i] = -1;

    // Create server msg queue
    key_t key = ftok(getenv(KEY_PATH_ENV), SERVER_KEYGEN_NUM);
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Server message queue created with ID: %d\n", msqid);

    msg_buf msg;
    while(1) {
        msgrcv(msqid, &msg, sizeof(msg_buf) - sizeof(long), 0, 0);
        switch (msg.mtype) {
            case INIT:
                INIT_handler(msg);
                break;
            case MESSAGE_ALL:
                MESSAGE_ALL_handler(msg);
                break;
            default:
                printf("Invalid message type: %ld\n", msg.mtype);
                continue;
        }
    }
}