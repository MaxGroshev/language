#include "disassembler.h"

int* read_bin_file ()
{
    FILE* num_com_bin = fopen ("../test-code.bin", "rb");
    MY_ASSERT (num_com_bin != NULL);

    struct stat stat_of_bin = {};
    stat ("../test-code.bin", &stat_of_bin);
    size_t size_of_bin = stat_of_bin.st_size;
    int*   code_of_com = (int*) calloc (size_of_bin + 1, sizeof (int));
    MY_ASSERT (code_of_com != NULL);
    fread (code_of_com, sizeof (int), size_of_bin, num_com_bin);

    fclose (num_com_bin);

    return code_of_com;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------

int disasm_com (int* code_of_com)
{
    FILE* disasm_prog = fopen ("../disasm.asm", "w");
    MY_ASSERT (disasm_prog != NULL)
    char* name_of_reg = (char*) calloc (10, sizeof (char));
    MY_ASSERT (name_of_reg != NULL);

    int elem         = 0;
    int code_of_reg  = 0;
    int label_of_jmp = 0;
    int i = 0;
    for (i = 0 ; code_of_com[i] != HLT; i++)
    {
        switch (code_of_com[i])
        {
            case PUSH:
                i++;
                elem = code_of_com[i];
                fprintf (disasm_prog, "push %d\n", elem);
                break;

            case IN:
                fprintf (disasm_prog, "in\r\n");
                break;

            case PUSHR:
                i++;
                code_of_reg = code_of_com[i];
                reg_def (code_of_reg, &name_of_reg);
                fprintf (disasm_prog, "push %s\r\n", name_of_reg);
                break;

            case PUSHM:
                i++;
                elem = code_of_com[i];
                fprintf (disasm_prog, "push [%d]\r\n", elem);
                break;

            case PUSHRM:
                i++;
                code_of_reg = code_of_com[i];
                reg_def (code_of_reg, &name_of_reg);
                fprintf (disasm_prog, "push [%s]\r\n", name_of_reg);
                break;

            case POP:
                i++;
                code_of_reg = code_of_com[i];
                reg_def (code_of_reg, &name_of_reg);
                fprintf (disasm_prog, "pop %s\r\n", name_of_reg);
                break;

            case ADD:
                fprintf (disasm_prog, "add\r\n");
                break;

            case MUL:
                fprintf (disasm_prog, "mul\r\n");
                break;

            case DIV:
                fprintf (disasm_prog, "div\r\n");
                break;

            case SQRT:
                fprintf (disasm_prog, "sqrt\r\n");
                break;

            case OUT:
                fprintf (disasm_prog, "out\r\n");
                break;

            case JMP:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "jmp :%d\r\n", label_of_jmp);
                i++; //to skip direction of jmp
                break;

            case JB:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "jb :%d\r\n", label_of_jmp);
                i++;
                break;

            case JBE:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "jbe :%d\r\n", label_of_jmp);
                i++;
                break;

            case JA:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "ja :%d\r\n", label_of_jmp);
                i++;
                break;

            case JAE:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "jae :%d\r\n", label_of_jmp);
                i++;
                break;

            case JE:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "je :%d\r\n", label_of_jmp);
                i++;
                break;

            case JNE:
                i++;

                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "jne :%d\r\n", label_of_jmp);
                i++;
                break;

            case CALL:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, "call :%d\r\n", label_of_jmp);
                i++;
                break;

            case RET:
                fprintf (disasm_prog, "ret\r\n");
                break;

            case ENTER:
                fprintf (disasm_prog, "\r\n");
                break;

            case LABEL:
                i++;
                label_of_jmp = code_of_com[i];
                fprintf (disasm_prog, ":%d\r\n", label_of_jmp);
                break;

            case HLT:
                fprintf (disasm_prog, "hlt\r\n");
                break;
        }
    }
    if (code_of_com[i] == HLT) fprintf (disasm_prog, "hlt\r\n");

    free   (name_of_reg);
    fclose (disasm_prog);
    return 0;
}

void reg_def (int code_of_reg, char** name_of_reg)
{
    switch (code_of_reg)
    {
        case ax:
            strcpy (*name_of_reg, "ax");
            break;

        case bx:
            strcpy (*name_of_reg, "bx");
            break;

        case cx:
            strcpy (*name_of_reg, "cx");
            break;

        case dx:
            strcpy (*name_of_reg, "dx");
            break;
    }
}

