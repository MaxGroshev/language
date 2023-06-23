#include "CPU.h"

int stack_dump (stack_t* box, const char* DUR_FILE, const char* FUNCTION, int LINE, int print_choice)
{
    int status = stack_check(box);
    if (status != 0)
    {
        FILE* print_fail = fopen ("../processor.txt", "a");
        MY_ASSERT (print_fail != NULL);
        fprintf (stderr,"Error of running program, check processor.txt! \007\n");
        fprintf (print_fail,"ERROR IN FILE %s, FUNCTION %s, LINE %d\n", DUR_FILE, FUNCTION, LINE);
        switch  (status)
        {
            case OUT_OF_STACK:
                fprintf (print_fail,"ERROR: Request to cell thar does not belong to stack\n");
                fprintf (print_fail,"%3ld [%d] - Current cell is out of stack\n", box->counter, box->data[box->counter]);
                print_choice = 1;
                break;

            case WRONG_INPUT:
                fprintf (print_fail,"ERROR: Was input wrong data\n");
                print_choice = 1;
                break;

            case ERR_NULL_DATA:
                fprintf (print_fail,"ERROR: Memory pointer point to NULL\n");
                print_choice = 1;
                break;

            case CANARY_A_IS_DEAD:
                fprintf (print_fail,"ERROR: Canary at the beginning of stack_t struct is dead\n");
                print_choice = 1;
                break;

            case CANARY_B_IS_DEAD:
                fprintf (print_fail,"ERROR: Canary at the end of stack_t struct is dead\n");
                print_choice = 1;
                break;

            case START_DATA_CANARY_IS_DEAD:
                fprintf (print_fail,"ERROR: Canary at the beginnig of stack is dead\n");
                print_choice = 1;
                break;

            case END_DATA_CANARY_IS_DEAD:
                fprintf (print_fail,"ERROR: Canary at the end of stack is dead\n");
                print_choice = 1;
                break;

        }
        fclose (print_fail);
        if (print_choice == 1) stack_print (box, print_choice);
    }

    return status;
}

void stack_print (stack_t* box, int print_choice)
{
    FILE* print_fail = fopen ("../processor.txt", "a");
    MY_ASSERT (print_fail != NULL);
    for (int i = 0; (i < box->capacity) && (print_choice == 1); i++)
    {
        if (i == box->counter)
        {
            fprintf (print_fail,"%3d [%d] - Current cell\n", i, box->data[i]);
            continue;
        }

        fprintf(print_fail,"%3d [%d]\n", i, box->data[i]);

        if (i + 1 == box->capacity)
        {
            fprintf (print_fail,"-----End of the current print-----\n");
        }
    }
    fclose (print_fail);
}



