#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "server.h"
#include "instruction.h"

#define BACKLOG 0

void* client_connected(void* new_fd) {
    int fd = *((int*) new_fd);
    char name[20];
    sprintf(name, "res/file_of_%d", fd);

    FILE* file = fopen(name, "w");

    int running = 1;
    while (running == 1) {
        char buffer[INSTRUCTION_LENGTH];
        int numbytes;
        if ((numbytes = read(fd, buffer, INSTRUCTION_LENGTH)) == -1) {
            perror("read_fct");
            exit(1);
        } else if (strncmp(buffer, ":q\n", 3) == 0 || numbytes == 0) {
            printf("Le client %d s'est déconnecté\n", fd);
            fflush(stdout);
            running = 0;
        } else {
            buffer[numbytes] = '\0';
            printf("Received message: %s", buffer);
            fprintf(file, "%s", buffer);
        }
    }
    fclose(file);
    pthread_exit(NULL);
}

int srv_run(int port) {
    int sockfd, new_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Listening for connections on port %d...\n", port);

    pthread_mutex_t mon_mutex = PTHREAD_MUTEX_INITIALIZER;

    int running = 1;
    while (running == 1) {
        pthread_mutex_lock(&mon_mutex);
        sin_size = sizeof(client_addr);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
            perror("accept_error");
            exit(1);
        }

        pthread_t pthread;
        pthread_create(&pthread, NULL, client_connected, ((void*) &new_fd));

        printf("Received connection from %s\n", inet_ntoa(client_addr.sin_addr));
        pthread_mutex_unlock(&mon_mutex);
    }

    close(new_fd);
    close(sockfd);

    return 0;
}