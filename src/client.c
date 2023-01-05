#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"
#include "instruction.h"

#define PORT 15360
#define SERVER_IP "127.0.0.1"

    // int test(void) {
    //     printf("Running client...\n");

    //     char ins_txt[INSTRUCTION_LENGTH + 1];
    //     ins_txt[INSTRUCTION_LENGTH] = '\0';
    //     while (strcmp(ins_txt, ":q") != 0){
    //         printf("> ");
    //         scanf("%s", ins_txt);



    //         // ins_t* ins = ins_parse(ins_txt);
    //         // printf("%d:%s\n", ins->content_size, ins->content);
    //         // int line_count;
    //         // char** lines = ins_render(ins, &line_count);

    //         // for (int i = 0; i < line_count; i++) {
    //         //     printf("%s\n", lines[i]);
    //         //     free(lines[i]);
    //         // }

    //         // free(lines);
    //     }



    //     return 0;
    // }


    int clt_run() {
    int sockfd;
    struct sockaddr_in server_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    if (send(sockfd, "Hello, server!", 15, 0) == -1) {
        perror("send");
        exit(1);
    }


    int running = 1;
    while (running == 1) {
        char message[INSTRUCTION_LENGTH];
        printf("> ");
        fgets(message, INSTRUCTION_LENGTH, stdin);
        if (strcmp(message, ":q\n") == 0)
        {
            running = 0;
        } else {
            send(sockfd, message, strlen(message), 0);
        }
    }

    // char buffer[1024];
    // int numbytes;
    // if ((numbytes = recv(sockfd, buffer, 1024, 0)) == -1) {
    //     perror("recv");
    //     exit(1);
    // }
    // buffer[numbytes] = '\0';
    // printf("Received message: %s\n", buffer);

    close(sockfd);

    return 0;
    }