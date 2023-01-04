#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "server.h"
#include "utils.h"

int main(int argc, char** argv) {
    int opt;
    typedef enum {CLIENT, SERVER} type_t;
    type_t type = CLIENT;

    while ((opt = getopt(argc, argv, ":sho:")) != -1) {
        switch(opt) {
            case 's':
                type = SERVER;
                break;
            case 'h':
                usage();
                break;
            case 'o':
                printf("%s\n", optarg);
                break;
            case '?':
                printf("Error: Unknown option: %c\n", optopt);
                usage();
                break;
        }
    }

    switch(type) {
        case CLIENT:
            clt_run();
            break;
        case SERVER:
            srv_run();
            break;
    }

    exit(EXIT_SUCCESS);
}
