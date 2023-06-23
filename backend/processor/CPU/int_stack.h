#pragma once
#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include "stack_check.h"
#include "../my_ASSERT.h"

//=================================================================================================================================

enum code_of_fails
{
    HASH_ERROR                = -8,
    END_DATA_CANARY_IS_DEAD   = -7,
    START_DATA_CANARY_IS_DEAD = -6,
    CANARY_B_IS_DEAD          = -5,
    CANARY_A_IS_DEAD          = -4,
    OUT_OF_STACK              = -3,
    WRONG_INPUT               = -2,
    ERR_NULL_DATA             = -1,
    STACK_IS_OKEY             =  0,
};

//---------------------------------------------------------------------------------------------------------------------------------

typedef int type_of_elem;
const int   CANARY_VAL = 0x10101010;

//--------------------------------------------------------------------------------

struct stack_t
{
    int           canary_a;
    type_of_elem* data;
    size_t        counter;
    size_t        capacity;
    size_t        hash_data;
    size_t        hash_counter;
    int           canary_b;
};

//-----------------------------------------------------------------------------------------------------------------------------------

void   stack_init   (stack_t* box, stack_t* func_ret);
void   set_canary   (stack_t* box); //include stack_t func_ret
void   stack_push   (stack_t* box, double element);
double stack_pop    (stack_t* box);
int    stack_check  (stack_t* box);
void   stack_delete (stack_t* box, stack_t* func_ret);
int    stack_dump   (stack_t* box, const char* DUR_FILE, const char* FUNCTION, int LINE, int print_choice);
void   stack_print  (stack_t* box, int print_choice);
void   stack_resize (stack_t* box);

//-------------------------------------------------------------------------------------------------------------------------------------


