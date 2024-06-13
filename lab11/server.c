#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define SV_SOCK_PATH "/tmp/sock"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    int socket;
    char id[20];
    int activated;
} Client;

Client clients[MAX_CLIENTS];

void send_to_all(const char *sender_id, const char *message) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].activated) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s: %s", sender_id, message);
            write(clients[i].socket, buffer, strlen(buffer)+1);
        }
    }
}

void send_to_one(const char *sender_id, const char *receiver_id, const char *message) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].activated && strcmp(clients[i].id, receiver_id) == 0) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "whisper %s: %s", sender_id, message);
            write(clients[i].socket, buffer, strlen(buffer)+1);
        }
    }
}

void list_clients(int cl_sock) {
    char buffer[BUFFER_SIZE] = "Active clients:\n";
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].activated) {
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    send(cl_sock, buffer, strlen(buffer)+1, 0);
}

void client_loop(int id) {
    char buffer[BUFFER_SIZE];
    ssize_t num_read;
    while ((num_read == read(clients[id].socket, buffer, BUFFER_SIZE)) > 0) {
        if (strncmp(buffer, "LIST", 4) == 0) {
            list_clients(clients[id].socket);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            send_to_all(clients[id].id, buffer+5);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            char *receiver_id = strtok(buffer+5, " ");
            char *msg = strtok(NULL, "");
            send_to_one(clients[id].id, receiver_id, msg);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Use case: %s address port", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *address = argv[1];
    const int port = atoi(argv[2]);
    int server_socket;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr.s_addr = inet_addr(address)
    };

    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("after bind");
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("after listen");
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket;
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
            perror("accpet");
            continue;
        }
        printf("User Accepted");
        int id = -1;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].activated) {
                id = i;
                break;
            }
        }
        if (id == -1) {
            char *msg = "Server full";
            write(client_socket, msg, strlen(msg)+1);
            close(client_socket);
            continue;
        }

        clients[id].socket = client_socket;
        read(client_socket, clients[id].id, sizeof(clients[id].id)-1);
        clients[id].activated = 1;
        pid_t child = fork();
        if (child == 0) {
            client_loop(id);
        }
    }
    close(server_socket);
    return 0;
}