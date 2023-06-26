#include "CPU.h"

void dispatch_task (stack_t* box, stack_t* func_ret, processor* cpu, double elem)
{
    int* code_of_com = read_bin_file (cpu);
    int  code_of_reg = 0;
    for (int i = 0; code_of_com[i] != HLT; i++)
    {
        switch (code_of_com[i])
        {
            case PUSH:
                i++;
                elem = code_of_com[i];
                stack_push (box, elem);
                break;

            case IN:
                scanf ("%lg", &elem);
                stack_push (box, elem);
                break;

            case PUSHR:
                i++;
                stack_push (box, cpu->registers[code_of_com[i]]);
                break;

            case PUSHM:
                i++;
                elem = code_of_com[i];
                MY_ASSERT  (elem <= cpu->T_RAM.capacity);
                stack_push (box, cpu->T_RAM.data[(int) elem]);
                break;

            case PUSHRM:
                i++;
                code_of_reg = code_of_com[i];
                stack_push (box, cpu->T_RAM.data[(int)cpu->registers[code_of_reg]]);
                break;

            case POP:
                i++;
                stack_pop (box);
                break;

            case POPR:
                i++;
                code_of_reg = code_of_com[i];
                cpu->registers[code_of_com[i]] = stack_pop (box);
                break;

            case POPM:
                i++;
                elem = stack_pop (box);
                cpu->T_RAM.data[code_of_com[i]] = elem;
                break;

            case POPRM:
                i++;
                MY_ASSERT ((int)cpu->registers[code_of_com[i]] <= cpu->T_RAM.capacity);
                cpu->T_RAM.data[(int)cpu->registers[code_of_com[i]]] = stack_pop (box);
                break;

            case ADD:
                stack_add (box, elem);
                break;

            case MUL:
                stack_mul (box, elem);
                break;

            case DIV:
                stack_div (box, elem);
                break;

            case SQRT:
                find_sqrt (box, elem);
                break;

            case OUT:
                elem = stack_pop (box);
                printf ("%lg\n", elem);
                break;

            case JMP:
                i += 2; //+2 want to skip name of label
                i = code_of_com[i];
                i--;
                break;

            case JB:
                if (stack_pop (box) < stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case JBE:
                if (stack_pop (box) <= stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case JA:
                if (stack_pop (box) > stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case JAE:
                if (stack_pop (box) >= stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case JE:
                if (stack_pop (box) == stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case JNE:
                if (stack_pop (box) != stack_pop (box))
                {
                    i += 2;
                    i = code_of_com[i];
                    i--;
                }
                else i += 2;
                break;

            case CALL:
                stack_push (func_ret, i + 3);
                i += 2;
                i = code_of_com[i];
                i--;
                break;

            case RET:
                i = stack_pop (func_ret);
                i--;
                break;

            case ENTER:
                break;

            case LABEL:
                i++;
                break;

            case HLT:
                break;
        }
    }
    free (code_of_com);
}

//------------------------------------------------------------------------------------------------------------------------------------------------

int* read_bin_file (processor* cpu)
{
    FILE* num_com_bin = fopen ("../prog_files/asm_code.bin", "rb");
    MY_ASSERT (num_com_bin != NULL);

    stat ("../prog_files/asm_code.bin", &cpu->stat_of_bin);
    size_t size_of_bin = cpu->stat_of_bin.st_size;
    int*   code_of_com = (int*) calloc (size_of_bin, sizeof (int));
    MY_ASSERT (code_of_com != NULL);
    fread (code_of_com, sizeof (int), size_of_bin, num_com_bin);

    fclose (num_com_bin);

    return code_of_com;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void stack_add (stack_t* box, double elem)
{
    elem =  stack_pop  (box);
    elem += stack_pop (box);
    stack_push (box, elem);
    STACK_CHECK
}

void stack_mul (stack_t* box, double elem)
{
    elem =  stack_pop (box);
    elem *= stack_pop (box);
    stack_push (box, elem);
    STACK_CHECK
}

void stack_div (stack_t* box, double elem)
{
    elem =  stack_pop (box);
    elem /= stack_pop (box);
    stack_push (box, elem);
    STACK_CHECK
}

void find_sqrt (stack_t* box, double elem)
{
    elem = stack_pop (box);
    elem = sqrt      (elem);
    stack_push       (box, elem);
}
