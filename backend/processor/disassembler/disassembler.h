#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "../my_ASSERT.h"

struct stat;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
    PUSHR  = 11,  //push of register
    PUSHM  = 12,  //push from RAM
    PUSHRM = 13,  //push from RAM from cell with num of reg
    POP    = 14,
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

//-------------------------------------------------------------------------------------------------------------------------------------------

int* read_bin_file ();
int  disasm_com    (int* code_of_com);
void reg_def       (int code_of_reg, char** name_of_reg);



