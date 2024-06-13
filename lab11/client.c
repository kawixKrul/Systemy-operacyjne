#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int server_socket;

void handle_SIGINT(int sig) {
    write(server_socket, "STOP", 5);
    close(server_socket);
    exit(EXIT_SUCCESS);
}

void receive_msg() {
    char buffer[BUFFER_SIZE];
    ssize_t num_read;
    while ((num_read == read(server_socket, buffer, BUFFER_SIZE)) > 0) {
        printf("%s\n", buffer);
    }
    printf("Connection ended");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Use case: %s client_id address port", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *client_id = argv[1];
    const char *address = argv[2];
    const int port = atoi(argv[3]);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr.s_addr = inet_addr(address)
    };

    if (connect(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    write(server_socket, client_id, strlen(client_id)+1);
    signal(SIGINT, handle_SIGINT);
    pid_t child = fork();
    if (child == 0) {
        receive_msg();
    } else {
        char buffer[BUFFER_SIZE];
        while(1) {
            fgets(buffer, BUFFER_SIZE, stdin);
            if (strcmp(buffer, "STOP") == 0) {
                raise(SIGINT);
            }
            write(server_socket, buffer, strlen(buffer)+1);
        }
    }
    close(server_socket);
    return 0;
}