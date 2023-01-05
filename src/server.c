#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 15360
#define BACKLOG 0

void* client_connected(void* new_fd) {
    int running = 1;
    while (running == 1) {
        char buffer[1024];
        int numbytes;
        if ((numbytes = recv(*((int*) new_fd), buffer, 1024, 0)) == -1) {
            perror("recv");
            exit(1);
        } else {
            buffer[numbytes] = '\0';
            if (strcmp(buffer, ":q\n") == 0) running = 0;
            printf("Received message: %s", buffer);
        }
    }    
    pthread_exit(NULL);
}

int srv_run() {
    int sockfd, new_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
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

    printf("Listening for connections on port %d...\n", PORT);

    int running = 1;
    while (running == 1) {
        sin_size = sizeof(client_addr);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
            perror("accept");
                exit(1);
        }

        pthread_t pthread;
        pthread_create(&pthread, NULL, client_connected, ((void*) &new_fd));

        printf("Received connection from %s\n", inet_ntoa(client_addr.sin_addr));
        
        
    }


    close(new_fd);
    close(sockfd);

    return 0;
}