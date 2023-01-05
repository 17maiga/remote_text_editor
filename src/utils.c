#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void usage() {
    printf("Usage: editor [options]\n\n");
    printf("============== Options: ==============\n");
    printf("|  -h      Print this help menu.     |\n");
    printf("|  -s      Start a server.           |\n");
    printf("|  -p      Choose the port.          |\n");
    printf("======================================\n\n");
    exit(EXIT_FAILURE);
}
