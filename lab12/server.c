#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in address;
    char id[20];
    int active;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void broadcast_message(const char *sender_id, const char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s: %s", sender_id, message);
            sendto(server_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&clients[i].address, sizeof(clients[i].address));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_to(const char *sender_id, const char *receiver_id, const char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && strcmp(clients[i].id, receiver_id) == 0) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s: %s", sender_id, message);
            sendto(server_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&clients[i].address, sizeof(clients[i].address));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int index) {
    clients[index].active = 0;
    memset(clients[index].id, 0, sizeof(clients[index].id));
}


void handle_message(char *buffer, struct sockaddr_in client_address) {
    pthread_mutex_lock(&clients_mutex);
    int index = -1;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && memcmp(&clients[i].address, &client_address, sizeof(client_address)) == 0) {
            index = i;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    if (index == -1) {
        // New client
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].active) {
                index = i;
                clients[i].address = client_address;
                strncpy(clients[i].id, buffer, sizeof(clients[i].id) - 1);
                clients[i].active = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    if (strncmp(buffer, "2ALL ", 4) == 0) {
        broadcast_message(clients[index].id, buffer + 5);
    } else if (strncmp(buffer, "2ONE ", 4) == 0) {
        char *receiver_id = strtok(buffer + 5, " ");
        char *message = strtok(NULL, "");
        send_message_to(clients[index].id, receiver_id, message);
    } else if (strncmp(buffer, "STOP", 4) == 0) {
        remove_client(index);
    }
}

void *ping_clients(void *arg) {
    while (1) {
        sleep(20);
        printf("PINGING CLIENTS\n");
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].active) {
                if (sendto(server_socket, "ALIVE", 5, 0, (struct sockaddr*)&clients[i].address, sizeof(clients[i].address)) <= 0) {
                    remove_client(i);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *address = argv[1];
    int port = atoi(argv[2]);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", address, port);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping_clients, NULL);

    while (1) {
        char buffer[BUFFER_SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int read_size = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (read_size > 0) {
            buffer[read_size] = '\0';
            handle_message(buffer, client_addr);
        }
    }

    close(server_socket);
    return 0;
}