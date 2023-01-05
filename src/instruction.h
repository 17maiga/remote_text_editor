#include <stdlib.h>

#ifndef REMOTE_TEXT_EDITOR_INSTRUCTION_H
#define REMOTE_TEXT_EDITOR_INSTRUCTION_H

#define INSTRUCTION_LENGTH 800
#define LINE_LENGTH 80
#define MIN_WRITING_LENGTH 5 // 5 is proposed as it is the minimum length required for a bulleted command with wrapped text
// > -t-
// > -e-
// > -s-
// > -t-

typedef enum ins_mode_e {NORMAL, LOWER, UPPER} ins_mode_t;

typedef struct ins_s {
    char* content;
    int content_size;
    ins_mode_t mode;
    int indent;
    int blocks;
    int bullet;
} ins_t;

/**
 * Creates an empty instruction in memory.
 *
 * @param[out] instruction
 */
ins_t* ins_create();

/**
 * Frees an instruction from memory.
 *
 * @param[in] instruction
 */
void ins_delete(ins_t* instruction);

/**
 * Sets the mode of an enriched text instruction.
 *
 * @param[in] instruction
 * @param[in] mode
 */
void ins_set_mode(ins_t* instruction, ins_mode_t mode);

/**
 * Opens a block in an enriched text instruction.
 *
 * @param[in] instruction
 */
void ins_open_block(ins_t* instruction);

/**
 * Opens n blocks in an enriched text instruction.
 *
 * @param[in] instruction
 * @param[in] n
 */
void ins_open_block_n(ins_t* instruction, int n);

/**
 * Closes a block in an enriched text instruction.
 *
 * @param[in] instruction
 */
void ins_close_block(ins_t* instruction);

/**
 * Closes n blocks in an enriched text instruction.
 *
 * @param[in] instruction
 * @param[in] n
 */
void ins_close_block_n(ins_t* instruction, int n);

/**
 * Indents the text in an enriched text instruction by 2 spaces.
 *
 * @param[in] instruction
 */
void ins_indent(ins_t* instruction);

/**
 * Indents the text in an enriched text instruction by n spaces.
 *
 * @param[in] instruction
 * @param[in] n
 */
void ins_indent_n(ins_t* instruction, int n);

/**
 * Unindents the text in an enriched text instruction by 2 spaces.
 *
 * @param[in] instruction
 */
void ins_unindent(ins_t* instruction);

/**
 * Unindents the text in an enriched text instruction by n spaces.
 *
 * @param[in] instruction
 * @param[in] n
 */
void ins_unindent_n(ins_t* instruction, int n);

/**
 * Sets an enriched text instruction to be a bullet point.
 *
 * @param[in] instruction
 */
void ins_bullet(ins_t* instruction);

typedef enum status_e {
    COMMAND,
    ARGUMENT,
    TEXT
} status_t;

typedef enum command_e {
    BLOCK,
    UNBLOCK,
    BULLET,
    INDENT,
    UNINDENT,
    MODE,
    WRITE
} command_t;

/**
 * Parses an instruction received by the server into an enriched text
 * instruction.
 *
 * @param[in]  text
 * @param[in]  prev_block_lvl
 * @param[in]  prev_indent_lvl
 * @param[out] instruction
 */
ins_t* ins_parse(char* text, int prev_block_lvl, int prev_indent_lvl);

/**
 * Transforms an enriched text instruction into the final text to be written.
 *
 * @param[in]  instruction
 * @param[in]  prev_block_lvl
 * @param[in]  prev_indent_lvl
 * @param[in]  line_count
 * @param[out] lines
 */
char** ins_render(ins_t* instruction, int prev_block_lvl, int* line_count);

#endif // REMOTE_TEXT_EDITOR_INSTRUCTION_H
