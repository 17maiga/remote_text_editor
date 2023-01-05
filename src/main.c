#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "server.h"
#include "utils.h"
#include "instruction.h"

int main(int argc, char** argv) {
    int port = 16859;
    int opt;
    typedef enum {CLIENT, SERVER} type_t;
    type_t type = CLIENT;

    while ((opt = getopt(argc, argv, ":sho:p:")) != -1) {
        switch(opt) {
            case 's':
                type = SERVER;
                break;
            case 'h':
                usage();
                break;
            case 'p':
                sscanf(optarg, "%d", &port);
                break;
            case '?':
                printf("Error: Unknown option: %c\n", optopt);
                usage();
                break;
        }
    }

    switch(type) {
        case CLIENT:
            clt_run(port);
            break;
        case SERVER:
            srv_run(port);
            break;
    }

    exit(EXIT_SUCCESS);
}
