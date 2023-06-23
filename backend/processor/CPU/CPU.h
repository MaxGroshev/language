#include "int_stack.h"
#include "RAM_functions.h"

static const size_t count_of_reg = 4;

//------------------------------------------------------------------------------------------------------------------------------------------------

struct processor
{
    double registers[4];     // four registers ax, bx, cx, dx
    struct stack_t box;      // stack with data
    struct stack_t func_ret; // stack with addreses of returning
    struct stat stat_of_bin; // statistic of file the file with bin commands
    struct RAM T_RAM;        // RAM
};

//------------------------------------------------------------------------------------------------------------------------------------------------

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
    JNE    = 26,  // =!
    CALL   = 30,
    RET    = 31,
    LABEL  = 32,
    ENTER  = 40,
};

//--------------------------------------------------------------------------------------

void dispatch_task (stack_t* box, stack_t* func_ret, processor* cpu, double elem);
int* read_bin_file (processor* cpu);
void stack_add     (stack_t* box, double elem);
void stack_mul     (stack_t* box, double elem);
void stack_div     (stack_t* box, double elem);
void find_sqrt     (stack_t* box, double elem);
