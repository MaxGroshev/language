#include "int_stack.h"

int stack_check (stack_t* box)
{
    if (box->data == NULL)
    {
        return ERR_NULL_DATA;
    }

    if (sizeof (box->data[box->counter - 1]) != sizeof (type_of_elem))
    {
        return WRONG_INPUT;
    }

    if (box->counter == -1)
    {
        return OUT_OF_STACK;
    }

    if (box->canary_a != CANARY_VAL)
    {
        return CANARY_A_IS_DEAD;
    }

    if (box->canary_b != CANARY_VAL)
    {
        return CANARY_B_IS_DEAD;
    }

    if (box->data[0] != CANARY_VAL)
    {
        return START_DATA_CANARY_IS_DEAD;
    }

    if (box->data[box->capacity] != CANARY_VAL)
    {
        return END_DATA_CANARY_IS_DEAD;
    }


    return STACK_IS_OKEY;

}

