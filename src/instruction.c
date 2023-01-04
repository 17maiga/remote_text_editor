#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"

ins_t* ins_create() {
    ins_t* instruction = malloc(sizeof(ins_t));
    instruction->content = malloc(INSTRUCTION_LENGTH * sizeof(char));
    instruction->content_size = 0;
    instruction->mode = NORMAL;
    instruction->indent = 0;
    instruction->blocks = 0;
    instruction->bullet = 0;
    return instruction;
}

void ins_delete(ins_t* instruction) {
    free(instruction);
}

void ins_set_mode(ins_t* instruction, ins_mode_t mode) {
    instruction->mode = mode;
    if (mode == UPPER)
        for (int i = 0; i < instruction->content_size; i++)
            instruction->content[i] = toupper(instruction->content[i]);
    else if (mode == LOWER)
        for (int i = 0; i < instruction->content_size; i++)
            instruction->content[i] = tolower(instruction->content[i]);
}

void ins_open_block(ins_t* instruction) {
    if (LINE_LENGTH - (instruction->blocks + 1) * 2 > 1)
        // There are enough spaces left in the line to write text
        instruction->blocks++;
}

void ins_open_block_n(ins_t* instruction, int n) {
    if (LINE_LENGTH - (instruction->blocks + n) * 2 > 1)
        instruction->blocks += n;
    else
        while (LINE_LENGTH - (instruction->blocks) * 2 > 2)
            instruction->blocks++;
}

void ins_close_block(ins_t* instruction) {
    if (instruction->blocks > 0)
        instruction->blocks--;
}

void ins_close_block_n(ins_t* instruction, int n) {
    if (instruction->blocks >= n)
        instruction->blocks -= n;
    else
        instruction->blocks = 0;
}

void ins_indent(ins_t* instruction) {
    instruction->indent += 2;
}

void ins_indent_n(ins_t* instruction, int n) {
    instruction->indent += n;
}

void ins_unindent(ins_t* instruction) {
    if (instruction->blocks >= 2)
        instruction->blocks -= 2;
    else
        instruction->blocks = 0;
}

void ins_unindent_n(ins_t* instruction, int n) {
    if (instruction->blocks >= n)
        instruction->blocks -= 2;
    else
        instruction->blocks = 0;
}

void ins_bullet(ins_t* instruction) {
    instruction->bullet = 1;
}

ins_t* ins_parse(char* text) {
    char buffer[5];
    size_t buflen = 0;
    status_t status = COMMAND;
    ins_t* instruction = ins_create();
    command_t command;

    for (int i = 0; i < (int) strlen(text); i++) {
        char c = text[i];
        switch(status) {
            case COMMAND:
                switch(c) {
                    case 'b':
                        command = BLOCK;
                        break;
                    case 'B':
                        command = UNBLOCK;
                        break;
                    case 'i':
                        command = INDENT;
                        break;
                    case 'I':
                        command = UNINDENT;
                        break;
                    case 'l':
                        command = BULLET;
                        break;
                    case 'm':
                        command = MODE;
                        break;
                    case 'w':
                        command = WRITE;
                        break;
                    case '-':
                        status = ARGUMENT;
                        break;
                    case ' ':
                        switch(command) {
                            case MODE:
                                // No argument specified, we default to normal.
                                ins_set_mode(instruction, NORMAL);
                                break;
                            case BLOCK:
                                ins_open_block(instruction);
                                break;
                            case UNBLOCK:
                                ins_close_block(instruction);
                                break;
                            case INDENT:
                                ins_indent(instruction);
                                break;
                            case UNINDENT:
                                ins_unindent(instruction);
                                break;
                            case BULLET:
                                ins_bullet(instruction);
                                break;
                            case WRITE:
                                status = TEXT;
                                break;
                        }
                        break;
                }
                break;
            case ARGUMENT:
                if (c == ' ') {
                    int n;
                    char m;
                    char tmp[buflen];
                    strncpy(tmp, buffer, buflen);
                    buflen = 0;
                    if (command != MODE) {
                        sscanf(tmp, "%d", &n);
                    } else {
                        sscanf(tmp, "%c", &m);
                        if (m == 'l')
                            n = LOWER;
                        else if (m == 'u')
                            n = UPPER;
                        else
                            n = NORMAL;
                    }
                    switch(command) {
                        case BLOCK:
                            ins_open_block_n(instruction, n);
                            break;
                        case UNBLOCK:
                            ins_close_block_n(instruction, n);
                            break;
                        case BULLET:
                            ins_bullet(instruction);
                            break;
                        case INDENT:
                            ins_indent_n(instruction, n);
                            break;
                        case UNINDENT:
                            ins_unindent_n(instruction, n);
                            break;
                        case MODE:
                            ins_set_mode(instruction, n);
                            break;
                        case WRITE:
                            status = TEXT;
                            break;
                    }
                    status = COMMAND;
                } else if (c < '0' || c > '9')
                    continue;
                else
                    buffer[buflen++] = c;
                break;
            case TEXT:
                instruction->content[instruction->content_size++] = c;
                break;
        }
    }
    printf("%d\n", instruction->content_size);
    return instruction;
}

char** ins_render(ins_t* instruction, int* line_count) {
    int writeable_length = LINE_LENGTH - (instruction->blocks * 2) -
        instruction->indent - (instruction->bullet * 2);
    printf("wlen: %d\n", writeable_length);
    *line_count = instruction->content_size / writeable_length +
        instruction->blocks * 2;
    if (instruction->content_size % writeable_length != 0)
        (*line_count)++;
    printf("%d\n", *line_count);
    char** lines = malloc(*line_count * sizeof(char*));
    int written_content = 0;
    int bullet_written = 0;
    int is_breaking_word = 0;
    for (int i = 0; i < *line_count; i++) {
        char* line = malloc((LINE_LENGTH + 1) * sizeof(char));
        line[LINE_LENGTH] = '\0';

        int writing_index = 0;

        // Open blocks
        if (i < instruction->blocks) {
            while (writing_index < i)
                line[writing_index++] = '|';
            line[writing_index++] = '+';
            while (writing_index < LINE_LENGTH - i - 1)
                line[writing_index++] = '-';
            line[writing_index++] = '+';
            while (writing_index < LINE_LENGTH)
                line[writing_index++] = '|';

        // Close blocks
        } else if (i > *line_count - instruction->blocks - 1) {
            while (writing_index < *line_count - i - 1)
                line[writing_index++] = '|';
            line[writing_index++] = '+';
            while (writing_index < LINE_LENGTH - (*line_count - i - 1) - 1)
                line[writing_index++] = '-';
            line[writing_index++] = '+';
            while (writing_index < LINE_LENGTH)
                line[writing_index++] = '|';

        // Write lines
        } else {
            // Open blocks
            while (writing_index < instruction->blocks)
                line[writing_index++] = '|';
            // Write indent
            while (writing_index < instruction->blocks + instruction->indent)
                line[writing_index++] = ' ';
            // Write bullet points if necessary
            if (instruction->bullet) {
                if (bullet_written == 0) {
                    switch(instruction->indent % 6) {
                        case 0:
                        case 1:
                            line[writing_index++] = '*';
                            break;
                        case 2:
                        case 3:
                            line[writing_index++] = '>';
                            break;
                        case 4:
                        case 5:
                            line[writing_index++] = '-';
                            break;
                    }
                    line[writing_index++] = ' ';
                    bullet_written = 1;
                } else {
                    line[writing_index++] = ' ';
                    line[writing_index++] = ' ';
                }
            }
            // Write lines
            if (instruction->content_size - written_content >
                writeable_length) {
                // We are not at the last written line

                // Write the first character of the line
                if (is_breaking_word == 1) {
                    line[writing_index++] = '-';
                    is_breaking_word = 0;
                } else {
                    while (instruction->content[written_content] == ' ')
                        written_content++;
                    line[writing_index++] =
                        instruction->content[written_content++];
                }
                // Write the rest of the line until the last character
                while (writing_index < LINE_LENGTH - instruction->blocks - 1) {
                    line[writing_index++] =
                        instruction->content[written_content++];
                }
                if (instruction->content[written_content] != ' ' &&
                    instruction->content[written_content + 1] != ' ') {
                    if (instruction->content[written_content - 1] == ' ') {
                        line[writing_index++] = ' ';
                    } else {
                        is_breaking_word = 1;
                        line[writing_index++] = '-';
                    }
                } else {
                    line[writing_index++] =
                        instruction->content[written_content++];
                }
            } else {
                // We are at the last written line

                // Write the first character of the line
                if (is_breaking_word == 1) {
                    line[writing_index++] = '-';
                    is_breaking_word = 0;
                } else {
                    while (instruction->content[written_content] == ' ')
                        written_content++;
                    line[writing_index++] =
                        instruction->content[written_content++];
                }
                // Write any remaining content
                while (written_content < instruction->content_size)
                    line[writing_index++] =
                        instruction->content[written_content++];
                // Fill the rest of the line with spaces.
                while (writing_index < LINE_LENGTH - instruction->blocks)
                    line[writing_index++] = ' ';
            }
            // Close blocks
            while (writing_index < LINE_LENGTH)
                line[writing_index++] = '|';
        }
        lines[i] = line;
    }
    return lines;
}
