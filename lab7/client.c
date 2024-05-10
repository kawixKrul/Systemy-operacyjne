#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "consts.h"

int msqid;
int key;
int client_id;
int server_msqid;

void clean_up(){
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        return;
    }
    printf("Message queue closed successfully\n");
}


void create_queue(){
    char rand_num = (rand() % CLIENT_KEYGEN_NUM_RANGE) + 2;
    key = ftok(getenv(KEY_PATH_ENV), rand_num);
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    printf("Client message queue created with ID: %d\n", msqid);
}

void stop(){
    exit(EXIT_SUCCESS);
}

void send_INIT(){
    msg_buf msg;
    msg.mtype = INIT;
    msg.value = key;
    strcpy(msg.text, "");
    msg.sender_id = -2;
    msgsnd(server_msqid, &msg, sizeof(msg_buf) - sizeof(long), 0);

    msg_buf response;
    msgrcv(msqid, &response, sizeof(msg_buf) - sizeof(long), INIT, 0);
    client_id = response.value;
    if (client_id < 0){
        printf("ERROR: Server is full\n");
        exit(EXIT_SUCCESS);
    }
    printf("Received client ID: %d\n", response.value);
}

void send_MESSAGE_ALL(char *text){
    msg_buf msg;
    msg.mtype = MESSAGE_ALL;
    msg.sender_id = client_id;
    msg.value = 0;
    strcpy(msg.text, text);
    msgsnd(server_msqid, &msg, sizeof(msg_buf) - sizeof(long), 0);
}

void MESSAGE_ALL_handler(msg_buf msg){
    printf("\nReceived message from client %d: %s\n", msg.sender_id, msg.text);
}

void SIGINT_handler(){
    printf("\n");
    stop();
}

void* input_function() {
    size_t len = 0;
    char *text = NULL;
    while (1) {
        printf("Enter message: ");
        getline(&text, &len, stdin);
        send_MESSAGE_ALL(text);
    }
}

int main (){
    srand(time(NULL));
    signal(SIGINT, SIGINT_handler);
    atexit(clean_up);

    // create client msg queue
    create_queue();

    // open server msg queue
    key_t server_key = ftok(getenv(KEY_PATH_ENV), SERVER_KEYGEN_NUM);
    server_msqid = msgget(server_key, 0666 | IPC_CREAT);

    // send init msg
    send_INIT();

    // input process
    pid_t pid = fork();
    if (pid == 0) {
        input_function();
        exit(EXIT_SUCCESS);
    }

    msg_buf msg;
    while(1) {
        msgrcv(msqid, &msg, sizeof(msg_buf) - sizeof(long), 0, 0);
        switch (msg.mtype) {
            case STOP:
                if (msg.sender_id == -1)
                    printf("\nReceived STOP from server\n");
                stop();
            case MESSAGE_ALL:
                MESSAGE_ALL_handler(msg);
                break;
        }
    }
}
