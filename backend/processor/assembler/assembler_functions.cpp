#include "assembler.h"

char* read_com_asm (FILE* word_com)
{
    struct stat stat_of_txt = {};
    stat   (NAME_OF_PROG, &stat_of_txt);
    size_t size_of_file = stat_of_txt.st_size;

    char*      asm_prog = (char*) calloc (size_of_file + 1, sizeof (char));
    MY_ASSERT (asm_prog != NULL);
    fread     (asm_prog, sizeof (char), size_of_file, word_com);
    return asm_prog;
}

//-----------------------------------------------------------------------------------------------------------------------

struct token* read_word_com (size_t* count_of_com, size_t* count_of_token, int* labels, char* asm_prog)
{
    int line = 0;
    size_t token_mem = 10;
    struct token* commands = (struct token*) calloc (token_mem, sizeof (struct token));
    MY_ASSERT (commands != NULL);

    char*  cur_tok  = strtok (asm_prog, " \n");
    MY_ASSERT (cur_tok != NULL);
    commands[0].com = cur_tok;
    for (int cur_elem = 1; cur_tok != NULL; cur_elem++) //TODO
    {
        if (token_mem <= cur_elem)
        {
            token_mem *= 2;
            struct token* commands_resize = commands;
            commands_resize = (struct token*) realloc (commands, token_mem * sizeof (struct token));
            MY_ASSERT (commands_resize != NULL);
            commands = commands_resize;
        }

        if (strcmp (commands[cur_elem - 1].com, "push") == 0)
        {
            push_def (commands, cur_tok, count_of_token, count_of_com, cur_elem);
        }
        else if (strcmp (commands[cur_elem - 1].com, "pop") == 0)
        {
            pop_def (commands, cur_tok, count_of_token, count_of_com, cur_elem);
        }
        else if ((strchr (commands[cur_elem - 1].com, 'j') != NULL) || (strcmp (commands[cur_elem - 1].com, "call") == 0))
        {
            cur_tok = strtok (NULL, " \n");
            commands[cur_elem - 1].label = cur_tok;
            (*count_of_token) += 2;
        }
        else if ((strchr (cur_tok, ':') != NULL) && (strchr (commands[cur_elem - 1].com, 'j') == NULL) && (strcmp (commands[cur_elem - 1].com, "call") != 0))
        {
            int num_of_label = 0;
            sscanf (cur_tok, ":%d", &num_of_label);
            labels[num_of_label] = *count_of_token;
            (*count_of_token)++;
            //(*count_of_token)++;
        }

        cur_tok = strtok (NULL, " \n");

        if (cur_tok != NULL)
        {
            commands[cur_elem].com = cur_tok;
            (*count_of_com)++;
            (*count_of_token)++;
            line++;
            commands[cur_elem].num_of_line = line;
        }
        else
        {
            break;
        }
    }
    (*count_of_com)++;
    (*count_of_token)++;
    return commands;
}

//-----------------------------------------------------------------------------------------------------------------------

void push_def (struct token* commands, char* cur_tok, size_t* count_of_token, size_t* count_of_com, int cur_elem)
{
    cur_tok = strtok (NULL, " \r\n\t");
    if (strchr (cur_tok, 'x') != NULL)
    {
        commands[cur_elem - 1].code_of_reg = register_def (cur_tok);

        if ((strchr (cur_tok, '[') != NULL) && (strchr (cur_tok, ']') != NULL)) //make awful push_def better
        {
            commands[cur_elem - 1].val = cur_tok;
            printf ("%s\n", commands[cur_elem - 1].val);

            if (strcmp (commands[cur_elem - 1].com, "push") == 0)
            {
                commands[cur_elem - 1].com = (char*) "pushrm";
            }
        }
        else if (strcmp (commands[cur_elem - 1].com, "push") == 0)
        {
            commands[cur_elem - 1].com = (char*) "pushr";
        }
    }
    else if ((strchr (cur_tok, '[') != NULL) && (strchr (cur_tok, ']') != NULL))
    {
        commands[cur_elem - 1].val = cur_tok;
        printf ("%s\n", commands[cur_elem - 1].val);

        if (strcmp (commands[cur_elem - 1].com, "push") == 0)
        {
            commands[cur_elem - 1].com = (char*) "pushm";
        }
    }
    else
    {
        commands[cur_elem - 1].val = cur_tok;
    }
    (*count_of_token)++;
}

//=============================================================================================================

void pop_def (struct token* commands, char* cur_tok, size_t* count_of_token, size_t* count_of_com, int cur_elem)
{
    cur_tok = strtok (NULL, " \r\n\t");
    if (strchr (cur_tok, 'x') != NULL)
    {
        commands[cur_elem - 1].code_of_reg = register_def (cur_tok);

        if ((strchr (cur_tok, '[') != NULL) && (strchr (cur_tok, ']') != NULL)) //make awful push_def better
        {
            commands[cur_elem - 1].val = cur_tok;
            printf ("%s\n", commands[cur_elem - 1].val);

            if (strcmp (commands[cur_elem - 1].com, "pop") == 0)
            {
                commands[cur_elem - 1].com = (char*) "poprm";
            }
        }
        else if (strcmp (commands[cur_elem - 1].com, "pop") == 0)
        {
            commands[cur_elem - 1].com = (char*) "popr";
        }
    }
    else if ((strchr (cur_tok, '[') != NULL) && (strchr (cur_tok, ']') != NULL))
    {
        commands[cur_elem - 1].val = cur_tok;
        printf ("%s\n", commands[cur_elem - 1].val);

        if (strcmp (commands[cur_elem - 1].com, "pop") == 0)
        {
            commands[cur_elem - 1].com = (char*) "popm";
        }
    }
    else
    {
        commands[cur_elem - 1].val = cur_tok;
    }
    (*count_of_token)++;
}

//-----------------------------------------------------------------------------------------------------------------------------

int register_def (char* cur_tok)
{
    if      (strstr (cur_tok, "ax") != 0) return ax;
    else if (strstr (cur_tok, "bx") != 0) return bx;
    else if (strstr (cur_tok, "cx") != 0) return cx;
    else if (strstr (cur_tok, "dx") != 0) return dx;
    else    return -1;
}

//===============================================================================================================================

//#ifdef COMMENT
void translate_com (struct token* commands, const size_t count_of_com, const size_t count_of_token, int* labels, char* asm_prog)
{
    FILE*      num_com     = fopen ("../../prog_files/asm_code.txt", "w");
    MY_ASSERT (num_com != NULL);
    FILE*      num_com_bin = fopen ("../../prog_files/asm_code.bin", "wb");
    MY_ASSERT (num_com_bin != NULL);

    int cmd_array[count_of_token];
    int cmd_size = 0;
    for (int cur_elem = 0; cur_elem < count_of_com && cmd_size < count_of_token; cur_elem++, cmd_size++)
    {
        int value = 0;
        int code_of_reg = -1;
        if (strcmp (commands[cur_elem].com, "push") == 0)
        {
            if ((strchr (commands[cur_elem].val, ']') == NULL) && (sscanf (commands[cur_elem].val, "%d", &value) == 1))
            {
                fprintf (num_com, "%d %d\n", PUSH, value);
                cmd_array[cmd_size] = PUSH;
                cmd_size++;
                cmd_array[cmd_size] = value;
            }
            else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].val);
        }
        else if (strcmp (commands[cur_elem].com, "in\r") == 0)
        {
            printf ("in\n");
            fprintf (num_com, "%d\n", IN);
            cmd_array[cmd_size] = IN;
        }
        else if ((strcmp (commands[cur_elem].com, "pushr") == 0) && (strchr (commands[cur_elem].com, '[') == NULL) && (strchr (commands[cur_elem].com, ']') == NULL))
        {
            if (commands[cur_elem].code_of_reg >= ax && commands[cur_elem].code_of_reg <= dx)
            {
                fprintf (num_com, "%d %d\n", PUSHR, commands[cur_elem].code_of_reg);
                cmd_array[cmd_size] = PUSHR;
                cmd_size++;
                cmd_array[cmd_size] = commands[cur_elem].code_of_reg;
            }
            else INPUT_ERR ("%s %d", commands[cur_elem].com, commands[cur_elem].code_of_reg);
        }
        else if (strcmp (commands[cur_elem].com, "pushm") == 0)
        {
            if (sscanf (commands[cur_elem].val, "[%d", &value) == 1)
            {
                printf ("%s", commands[cur_elem].val);
                fprintf (num_com, "%d %d\n", PUSHM, value);
                cmd_array[cmd_size] = PUSHM;
                cmd_size++;
                cmd_array[cmd_size] = value;
            }
            else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].val);
        }
        else if (strcmp (commands[cur_elem].com, "pushrm") == 0)
        {
            if (commands[cur_elem].code_of_reg >= ax && commands[cur_elem].code_of_reg <= dx)
            {
                fprintf (num_com, "%d %d\n", PUSHRM, commands[cur_elem].code_of_reg);
                cmd_array[cmd_size] = PUSHRM;
                cmd_size++;
                cmd_array[cmd_size] = commands[cur_elem].code_of_reg;
            }
            else INPUT_ERR ("%s %d", commands[cur_elem].com, commands[cur_elem].code_of_reg);
        }
        else if (strcmp (commands[cur_elem].com, "pop") == 0) //improve
        {
            if (commands[cur_elem].code_of_reg >= ax && commands[cur_elem].code_of_reg <= dx)
            {
                fprintf (num_com, "%d %d\n", POP, commands[cur_elem].code_of_reg);
                cmd_array[cmd_size] = POP;
                cmd_size++;
                cmd_array[cmd_size] = commands[cur_elem].code_of_reg;
            }
            else INPUT_ERR ("%s %d", commands[cur_elem].com, commands[cur_elem].code_of_reg);
        }
        else if ((strcmp (commands[cur_elem].com, "popr") == 0) && (strchr (commands[cur_elem].com, '[') == NULL) && (strchr (commands[cur_elem].com, ']') == NULL))
        {
            if (commands[cur_elem].code_of_reg >= ax && commands[cur_elem].code_of_reg <= dx)
            {
                fprintf (num_com, "%d %d\n", POPR, commands[cur_elem].code_of_reg);
                cmd_array[cmd_size] = POPR;
                cmd_size++;
                cmd_array[cmd_size] = commands[cur_elem].code_of_reg;
            }
            else INPUT_ERR ("%s %d", commands[cur_elem].com, commands[cur_elem].code_of_reg);
        }
        else if (strcmp (commands[cur_elem].com, "popm") == 0)
        {
            if (sscanf (commands[cur_elem].val, "[%d", &value) == 1)
            {
                printf ("%s", commands[cur_elem].val);
                fprintf (num_com, "%d %d\n", POPM, value);
                cmd_array[cmd_size] = POPM;
                cmd_size++;
                cmd_array[cmd_size] = value;
            }
            else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].val);
        }
        else if (strcmp (commands[cur_elem].com, "poprm") == 0)
        {
            if (commands[cur_elem].code_of_reg >= ax && commands[cur_elem].code_of_reg <= dx)
            {
                fprintf (num_com, "%d %d\n", POPRM, commands[cur_elem].code_of_reg);
                cmd_array[cmd_size] = POPRM;
                cmd_size++;
                cmd_array[cmd_size] = commands[cur_elem].code_of_reg;
            }
            else INPUT_ERR ("%s %d", commands[cur_elem].com, commands[cur_elem].code_of_reg);
        }
        else if (strchr (commands[cur_elem].com, 'j') != NULL)
        {
            jmp_def (num_com ,commands, labels, cmd_array, &cmd_size, cur_elem);
        }
        else if (strcmp (commands[cur_elem].com, "call") == 0)
        {
            int num_of_label = 0;
            if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
            {
                fprintf (num_com, "%d :%d %d\n", CALL, num_of_label, labels[num_of_label]);
                cmd_array[cmd_size] = CALL;
                cmd_size++;
                cmd_array[cmd_size] = num_of_label;
                cmd_size++;
                cmd_array[cmd_size] = labels[num_of_label];
            }
            else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
        }
        else if (strcmp (commands[cur_elem].com, "ret\r") == 0)
        {
            fprintf (num_com, "%d\n", RET);
            cmd_array[cmd_size] = RET;
        }
        else if (strcmp (commands[cur_elem].com, "add\r") == 0)
        {
            fprintf (num_com, "%d\n", ADD);
            cmd_array[cmd_size] = ADD;
        }
        else if (strcmp (commands[cur_elem].com, "mul\r") == 0)
        {
            fprintf (num_com, "%d\n", MUL);
            cmd_array[cmd_size] = MUL;
        }
        else if (strcmp (commands[cur_elem].com, "div\r") == 0)
        {
            fprintf (num_com, "%d\n", DIV);
            cmd_array[cmd_size] = DIV;
        }
        else if (strcmp (commands[cur_elem].com, "out\r") == 0)
        {
            fprintf (num_com, "%d\n", OUT);
            cmd_array[cmd_size] = OUT;
        }
        else if (strcmp (commands[cur_elem].com, "sqrt\r") == 0)
        {
            fprintf (num_com, "%d\n", SQRT);
            cmd_array[cmd_size] = SQRT;
        }
        else if (strcmp (commands[cur_elem].com, "hlt\r") == 0)
        {
            fprintf (num_com, "%d\n", HLT);
            cmd_array[cmd_size] = HLT;
        }
        else if (strcmp (commands[cur_elem].com, "\r") == 0)
        {
            fprintf (num_com, "\n");
            cmd_array[cmd_size] = ENTER;
        }
        else if (strchr (commands[cur_elem].com, ':') != NULL)
        {
            int num_of_label = 0;
            sscanf (commands[cur_elem].com, ":%d", &num_of_label) == 1;
            fprintf (num_com, ":%d\n", num_of_label);
            cmd_array[cmd_size] = LABEL;
            cmd_size++;
            cmd_array[cmd_size] = num_of_label;
            continue;
        }
        else INPUT_ERR("%s", commands[cur_elem].com)
    }

    fprintf(num_com, "------End of list of commands------\n");
    fwrite (cmd_array, sizeof (int), cmd_size , num_com_bin);
    fclose (num_com);
    fclose (num_com_bin);
}// make define with names of commands or array with name

//--------------------------------------------------------------------------------------------------------------------------------

void jmp_def (FILE* num_com, struct token* commands, int* labels, int* cmd_array, int* cmd_size, int cur_elem)
{
    int num_of_label = 0;
    if (strcmp (commands[cur_elem].com, "jmp") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JMP, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JMP;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "jb") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JB, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JB;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "jbe") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JBE, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JBE;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "ja") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JA, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JA;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "jae") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JAE, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JAE;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "je") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JE, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JE;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label);
    }
    else if (strcmp (commands[cur_elem].com, "jne") == 0)
    {
        if (sscanf (commands[cur_elem].label, ":%d", &num_of_label) == 1)
        {
            fprintf (num_com, "%d :%d %d\n", JNE, num_of_label, labels[num_of_label]);
            cmd_array[*cmd_size] = JNE;
            (*cmd_size)++;
            cmd_array[*cmd_size] = num_of_label;
            (*cmd_size)++;
            cmd_array[*cmd_size] = labels[num_of_label];
        }
        else INPUT_ERR ("%s %s", commands[cur_elem].com, commands[cur_elem].label)
    }
}
//#endif
