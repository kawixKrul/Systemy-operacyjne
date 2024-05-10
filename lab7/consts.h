//
// Created by Michał Kawa on 09/05/2024.
//

#ifndef SYSOP_CONSTS_H
#define SYSOP_CONSTS_H

#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_TEXT_SIZE 1024

// message types
enum msg_type {STOP, LIST, INIT, MESSAGE_ALL, MESSAGE_ONE};

// queue config
#define KEY_PATH_ENV "HOME"
#define SERVER_KEYGEN_NUM 1
#define CLIENT_KEYGEN_NUM_RANGE 200

typedef struct {
    long mtype;
    int value;
    int sender_id;
    char text[MSG_TEXT_SIZE];
} msg_buf;

#endif //SYSOP_CONSTS_H
