#include <stdio.h>
#include <string.h>

#include "client.h"
#include "instruction.h"

int clt_run(void) {
    printf("Running client...\n");

    char ins_txt[INSTRUCTION_LENGTH + 1];
    ins_txt[INSTRUCTION_LENGTH] = '\0';
    strcpy(ins_txt, "b-2 m-n l w There once was a ship that put to sea, the name of the ship was the Billy O'Tea");
    ins_t* ins = ins_parse(ins_txt);
    printf("%d:%s\n", ins->content_size, ins->content);
    int line_count;
    char** lines = ins_render(ins, &line_count);

    for (int i = 0; i < line_count; i++) {
        printf("%s\n", lines[i]);
        free(lines[i]);
    }

    free(lines);

    return 0;
}
