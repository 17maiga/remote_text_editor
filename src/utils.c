#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void usage() {
    printf("Usage: editor [options]\n\n");
    printf("================== Options: ==================\n");
    printf("|  -h         Print this help menu.          |\n");
    printf("|  -s         Start a server.                |\n");
    printf("|  -p         Choose the port.               |\n");
    printf("==============================================\n\n");
    printf("================== Commands ==================\n");
    printf("|In the client, it is possible to use        |\n");
    printf("|the following commands:                     |\n");
    printf("|                                            |\n");
    printf("| w           Write all remaining text       |\n");
    printf("| b[-n]       Open 1|n blocks                |\n");
    printf("| B[-n]       Close 1|n blocks               |\n");
    printf("| i[-n]       Indent 2|n spaces              |\n");
    printf("| I[-n]       Unindent 2|n spaces            |\n");
    printf("| l           Treat the line as a list       |\n");
    printf("| m[-n|-u|-l] Normal | Uppercase | Lowercase |\n");
    printf("| q           Exit the session               |\n");
    printf("==============================================\n\n");
}
