#pragma once
#define DEBUG
//#define COMMENT
#define NAME_OF_PROG "../prog_files/prog_code.asm"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "input_err.h"
#include "../my_ASSERT.h"

struct stat;

//-------------------------------------------------------------------------------------------------------------------------------

struct token
{
    char*  com;
    char*  val;
    int    code_of_reg;
    int    num_of_line;
    char*  label;
};

//===============================================================================================================================

enum code_of_com
{
    HLT    = 0,
    PUSH   = 1,   //classic push
    IN     = 2,
    ADD    = 3,
    MUL    = 4,
    OUT    = 5,
    DIV    = 6,
    SQRT   = 7,
    SUB    = 8,
    PUSHR  = 11,  //push of register
    PUSHM  = 12,  //push from RAM
    PUSHRM = 13,  //push from RAM from cell with num of reg
    POP    = 14,
    POPR   = 15,
    POPM   = 16,  //pop from RAM
    POPRM  = 17,  //pop from RAM from cell
    JMP    = 20,  // jmp without condition
    JB     = 21,  // <
    JBE    = 22,  // <=
    JA     = 23,  // >
    JAE    = 24,  // >=
    JE     = 25,  // ==
    JNE    = 26,  // !=
    CALL   = 30,
    RET    = 31,
    LABEL  = 32,
    ENTER  = 40,
};

enum registers
{
    ax = 100,
    bx = 101,
    cx = 102,
    dx = 103,
};

//==================================================================================================================================

char*  read_com_asm         (FILE* word_com);
struct token* read_word_com (size_t* count_of_com, size_t* count_of_token, int* labels, char* asm_prog);
void   translate_com        (struct token* commands, const size_t count_of_com, const size_t count_of_token, int* labels, char* asm_prog);
void   push_def             (struct token* commands, char* cur_tok, size_t* count_of_token, size_t* count_of_com, int cur_elem);
void   pop_def              (struct token* commands, char* cur_tok, size_t* count_of_token, size_t* count_of_com, int cur_elem);
int    register_def         (char* cur_tok);
void   jmp_def              (FILE* num_com, struct token* commands, int* labels, int* cmd_array, int* cmd_size, int cur_elem);
