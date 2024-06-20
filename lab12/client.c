#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int server_socket;
struct sockaddr_in server_addr;

void handle_exit(int sig) {
    sendto(server_socket, "STOP", 5, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    close(server_socket);
    exit(0);
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    while (1) {
        int read_size = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (read_size <= 0) {
            printf("Connection lost\n");
            exit(1);
        }
        if (strncmp(buffer, "ALIVE", 5) != 0) {
            buffer[read_size] = '\0';
            printf("%s\n", buffer);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client_id> <server_address> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *client_id = argv[1];
    const char *server_address = argv[2];
    int server_port = atoi(argv[3]);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_address);
    server_addr.sin_port = htons(server_port);

    sendto(server_socket, client_id, strlen(client_id) + 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    signal(SIGINT, handle_exit);

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "STOP", 4) == 0) {
            handle_exit(0);
        }
        sendto(server_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    }

    close(server_socket);
    return 0;
}