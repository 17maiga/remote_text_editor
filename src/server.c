#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#include "server.h"
#include "instruction.h"

#define BACKLOG 0

void* client_connected(void* new_fd) {
    int fd = *((int*) new_fd);
    char name[20];
    int block_level = 0;
    int indentation_level = 0;
    struct stat st = {0};
    if (stat("res", &st) == -1)
        mkdir("res", 0700);
    sprintf(name, "res/file_%d.txt", fd);

    FILE* file = fopen(name, "w");

    int running = 1;
    while (running == 1) {
        char buffer[INSTRUCTION_LENGTH];
        int numbytes;
        if ((numbytes = read(fd, buffer, INSTRUCTION_LENGTH)) == -1) {
            perror("read_fct");
            exit(1);
        } else if (strncmp(buffer, "q\n", 2) == 0 || numbytes == 0) {
            printf("Le client %d s'est déconnecté\n", fd);
            fflush(stdout);
            // Close any remaining blocks
            while (block_level > 0) {
                for (int i = 0; i < block_level - 1; i++)
                    fprintf(file, "|");
                fprintf(file, "+");
                for (int i = 0; i < LINE_LENGTH - (2*block_level); i++)
                    fprintf(file, "-");
                fprintf(file, "+");
                for (int i = 0; i < block_level - 1; i++)
                    fprintf(file, "|");
                fprintf(file, "\n");
                block_level--;
            }
            running = 0;
        } else {
            buffer[numbytes] = '\0';
            if (buffer[numbytes - 1] == '\n')
                buffer[numbytes - 1] = '\0';
            ins_t* ins = ins_parse(buffer, block_level, indentation_level);
            int line_count;
            char** lines = ins_render(ins, block_level, &line_count);
            block_level = ins->blocks;
            indentation_level = ins->indent;
            for (int i = 0; i < line_count; i++)
                fprintf(file, "%s\n", lines[i]);
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