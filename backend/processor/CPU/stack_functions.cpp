#include "CPU.h"

void stack_init (stack_t* box, stack_t* func_ret)
{
    box->capacity  = 50;
    box->data      = (type_of_elem*) calloc (box->capacity + 2, sizeof (type_of_elem));
    MY_ASSERT        (box->data != NULL);
    set_canary       (box);

    func_ret->data = (type_of_elem*) calloc (10, sizeof (type_of_elem)); //size of ret value is not more then 10
    MY_ASSERT        (func_ret->data != NULL);
    set_canary       (func_ret);
    STACK_CHECK
}

//-------------------------------------------------------------------------------------------------------------------------------------------

void set_canary (stack_t* box) // this name is ok or not?
{
    if (box->counter == 0)
    {
        box->data[0]             = CANARY_VAL;
        box->data[box->capacity] = CANARY_VAL;
        box->canary_a            = CANARY_VAL;
        box->canary_b            = CANARY_VAL;
        box->counter++;
    }

    else
    {
        box->data[0]             = CANARY_VAL;
        box->data[box->capacity] = CANARY_VAL;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void stack_push (stack_t*  box, double element)
{
    STACK_CHECK

    if (box->capacity  - 2 <= box->counter)
    {
        stack_resize (box);
    }
    box->data[box->counter++] = element * 100;
    STACK_CHECK
}

//----------------------------------------------------------------------------------------------------------------------------------------------

double stack_pop (stack_t* box)
{

    STACK_CHECK
    double element = box->data[--box->counter];
    element = element / 100;
    box->data[box->counter] = 0X7777777;
    STACK_CHECK

    return element;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void stack_resize (stack_t* box)
{
    box->capacity *= 2;
    int* stack_resize = box->data;
    stack_resize = (type_of_elem*) realloc (stack_resize , (box->capacity + 1) * sizeof (type_of_elem));
    MY_ASSERT (stack_resize != NULL);
    box->data = stack_resize;
    set_canary (box);
}

//---------------------------------------------------------------------------------------------------------------------------

void stack_delete (stack_t* box, stack_t* func_ret)
{
    free (box->data);
    free (func_ret->data);
}




