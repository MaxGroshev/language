#include "frontend.h"

//=======================================READING_OF_PROG_FILE=========================================================

char* read_file (const char* file_dir)
{
    FILE* seq_file = fopen (file_dir, "r");
    MY_ASSERT (seq_file != NULL);

    struct stat file_data = {};
    stat (file_dir, &file_data);
    char* buffer = (char*) calloc (file_data.st_size, sizeof (char));
    MY_ASSERT (buffer != NULL);

    fread  (buffer, sizeof (char), file_data.st_size, seq_file);
    fclose (seq_file);
    return buffer;
}

//===========================================LEXICAL_ANALYSIS=============================================================

int lexical_analysis (char* buffer, lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    for (int pos_in_buf = 0; buffer[pos_in_buf] != '\0';)
    {
        int start_pos = pos_in_buf;
        if (lex_stat->lex_capacity <= lex_stat->lex_size + 10) lexems_resize (lex_stat);
        if (buffer[pos_in_buf] == '$')
        {
            lex_stat->lexems[lex_stat->lex_size].node_type = OP_PROG_END;
            lex_stat->lex_size++;
            break;
        }
        while (buffer[pos_in_buf] == 32) pos_in_buf++;
        if    (buffer[pos_in_buf] == '\n' || buffer[pos_in_buf] == '\r' || buffer[pos_in_buf] == '\t')
        {
            prog_stat->str_num++;
            pos_in_buf++;
            continue;
        }

        is_this_op ("if",     OP_IF,     buffer, &pos_in_buf, lex_stat); // make end node
        is_this_op ("end!",   OP_END,    buffer, &pos_in_buf, lex_stat);
        is_this_op ("begin:", OP_BEGIN,  buffer, &pos_in_buf, lex_stat);
        is_this_op ("then",   OP_THEN,   buffer, &pos_in_buf, lex_stat);
        is_this_op ("return", OP_RETURN, buffer, &pos_in_buf, lex_stat);

        if (l_strncomp (buffer + pos_in_buf, "print", strlen ("print"), STR_SKIP_SPACE, &pos_in_buf))
        {
            add_std_lib_func (lex_stat, LIB_PRINT);
        }
        if (l_strncomp (buffer + pos_in_buf, "writeln", strlen ("writeln"), STR_SKIP_SPACE, &pos_in_buf))
        {
            add_std_lib_func (lex_stat, LIB_WRITELN);
        }
        if (l_strncomp (buffer + pos_in_buf, "sqrt", strlen ("sqrt"), STR_SKIP_SPACE, &pos_in_buf))
        {
            add_std_lib_func (lex_stat, LIB_SQR);
        }
        if (l_strncomp   (buffer + pos_in_buf, "var", strlen ("var"), STR_SKIP_SPACE, &pos_in_buf))
        {
            add_new_var  (buffer, &pos_in_buf, prog_stat, lex_stat);
        }
        if (l_strncomp   (buffer + pos_in_buf, "func_", strlen ("func_"), STR_SKIP_SPACE, &pos_in_buf))
        {
            add_new_func (buffer, &pos_in_buf, prog_stat, lex_stat);
        }
        if (buffer[pos_in_buf] >= '0' && buffer[pos_in_buf] <= '9')
        {
            add_new_num (buffer, &pos_in_buf, prog_stat, lex_stat);
        }
        if ((buffer[pos_in_buf] >= 'a' && buffer[pos_in_buf] <= 'z') ||
            (buffer[pos_in_buf] >= 'A' && buffer[pos_in_buf] <= 'Z'))
        {
            def_var_or_func (buffer, &pos_in_buf, prog_stat, lex_stat);
        }

        is_this_op (";",  OP_GART_N,   buffer, &pos_in_buf, lex_stat);
        is_this_op ("(",  OP_OPEN_BR,  buffer, &pos_in_buf, lex_stat);
        is_this_op (")",  OP_CLOSE_BR, buffer, &pos_in_buf, lex_stat);

        is_this_op ("==", OP_COMP_EQ,  buffer, &pos_in_buf, lex_stat);
        is_this_op ("!=", OP_N_COMP_EQ,buffer, &pos_in_buf, lex_stat);
        is_this_op (">=", OP_ABOVE_EQ, buffer, &pos_in_buf, lex_stat);
        is_this_op ("<=", OP_LESS_EQ,  buffer, &pos_in_buf, lex_stat);
        is_this_op (">",  OP_ABOVE,    buffer, &pos_in_buf, lex_stat);
        is_this_op ("<",  OP_LESS,     buffer, &pos_in_buf, lex_stat);
        is_this_op ("=",  OP_EQ,       buffer, &pos_in_buf, lex_stat);

        is_this_op ("+",  OP_ADD,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("-",  OP_SUB,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("*",  OP_MUL,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("/",  OP_DIV,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("^",  OP_POW,      buffer, &pos_in_buf, lex_stat);
        is_this_op (",",  OP_COMMA,    buffer, &pos_in_buf, lex_stat);

        if (start_pos == pos_in_buf) syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    }
    return 0;
}

int is_negative_val (lex_stat_t* lex_stat)
{
    if (lex_stat->lexems[lex_stat->lex_size - 1].node_type != TYPE_VAR &&
        lex_stat->lexems[lex_stat->lex_size - 1].node_type != TYPE_NUM &&
        lex_stat->lexems[lex_stat->lex_size - 1].node_type != TYPE_FUNC)

    {
        lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_NUM;
        lex_stat->lexems[lex_stat->lex_size].value     = -1;
        lex_stat->lex_size++;

        lex_stat->lexems[lex_stat->lex_size].node_type = OP_MUL;
        lex_stat->lex_size++;
        return 1;
    }
    return 0;
}

int is_exist_name (prog_data_t* prog_stat, const char* name, int type)
{
    if (type == TYPE_VAR)
    {
        for (int num_of_var = 0; num_of_var < prog_stat->var_num; num_of_var++)
        {
            MY_ASSERT (prog_stat->decl_vars != NULL)
            if (strcmp (name, prog_stat->decl_vars[num_of_var].name) == 0)
            {
                printf ("COMPARED by name: %s\n", prog_stat->decl_vars[num_of_var].name);
                return num_of_var;
            }
        }
        return VAR_NOT_EXIST;
    }
    else if (type == TYPE_FUNC)
    {
        for (int num_of_func = 0; num_of_func < prog_stat->func_num; num_of_func++)
        {
            MY_ASSERT (prog_stat->decl_funcs != NULL)
            if (strcmp (name, prog_stat->decl_funcs[num_of_func].name) == 0)
            {
                printf ("COMPARED by name: %s\n", prog_stat->decl_funcs[num_of_func].name);
                return num_of_func;
            }
        }
        return FUNC_NOT_EXIST;
    }
    return 0;
}
//=================================================================================================================================

int def_var_or_func (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    int i = 0;
    for (; (buffer[*pos_in_buf] >= 'a' && buffer[*pos_in_buf] <= 'z') ||
           (buffer[*pos_in_buf] >= 'A' && buffer[*pos_in_buf] <= 'Z'); i++, (*pos_in_buf)++)
    {
        if (i == BUF_OF_64_ELEM)
                syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
        lex_stat->lexems[lex_stat->lex_size].name[i] = buffer[*pos_in_buf];
    }
    lex_stat->lexems[lex_stat->lex_size].name[i] = '\0';

    int num_of_var = is_exist_name (prog_stat, lex_stat->lexems[lex_stat->lex_size].name, TYPE_VAR);
    if (num_of_var != VAR_NOT_EXIST)
    {
        add_exist_var (buffer, pos_in_buf, prog_stat, lex_stat, num_of_var);
        return 1;
    }
    int num_of_func = is_exist_name (prog_stat, lex_stat->lexems[lex_stat->lex_size].name, TYPE_FUNC);
    if (num_of_var != FUNC_NOT_EXIST)
    {
        add_exist_func (buffer, pos_in_buf, prog_stat, lex_stat, num_of_func);
        return 1;
    }

    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    return 0;
}


//=============================================ADDING_OF_MENTIONED_NAME_ELEMENTS=======================================================

int add_exist_var (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat, int num_of_var)
{
    if (lex_stat->lex_capacity <= lex_stat->lex_size + 10) lexems_resize (lex_stat);

    lex_stat->lexems[lex_stat->lex_size].node_type  = TYPE_VAR;
    lex_stat->lexems[lex_stat->lex_size].decloration= L_MENTION;
    lex_stat->lexems[lex_stat->lex_size].num_of_var_func = num_of_var;
    lex_stat->lex_size++;

    return 0;
}

int add_exist_func (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat, int num_of_func)
{
    if (lex_stat->lex_capacity <= lex_stat->lex_size + 10) lexems_resize (lex_stat);

    lex_stat->lexems[lex_stat->lex_size].node_type  = TYPE_FUNC;
    lex_stat->lexems[lex_stat->lex_size].decloration= L_MENTION;
    lex_stat->lexems[lex_stat->lex_size].num_of_var_func = num_of_func;
    lex_stat->lex_size++;

    return 0;
}

int is_this_op (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat)
{
    if (lex_stat->lex_capacity <= lex_stat->lex_size + 10) lexems_resize (lex_stat);

    if (l_strncomp (buffer + *pos_in_buf, str, strlen (str), STR_SKIP_SPACE, pos_in_buf))
    {
        if (code_of_op == OP_SUB && is_negative_val (lex_stat))
        {
            return 1;
        }
        lex_stat->lexems[lex_stat->lex_size].node_type = code_of_op;
        lex_stat->lex_size++;

        return 1;
    }
    return 0;
}

int add_std_lib_func (lex_stat_t* lex_stat, int code_of_func)
{
    lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_FUNC;
    lex_stat->lexems[lex_stat->lex_size].value     = code_of_func;
    lex_stat->lex_size++;

    return 0;
}

//=============================================ADDING_OF_NEW_NAME_ELEMENTS=======================================================

int add_new_num (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_NUM;
    lex_stat->lexems[lex_stat->lex_size].value     = my_strtod (buffer + *pos_in_buf, pos_in_buf);
    lex_stat->lex_size++;
    printf ("%lg\n", lex_stat->lexems[lex_stat->lex_size - 1].value);

    return 0;
}

int add_new_var (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    if (prog_stat->var_capacity <= prog_stat->var_num + 10) prog_stat_resize (prog_stat);
    if (lex_stat->lex_capacity  <= lex_stat->lex_size + 10) lexems_resize    (lex_stat);

    int  start_pos = *pos_in_buf;
    int  i = 0;
    for (;(buffer[*pos_in_buf] >= 'a' && buffer[*pos_in_buf] <= 'z') ||
          (buffer[*pos_in_buf] >= 'A' && buffer[*pos_in_buf] <= 'Z'); i++, (*pos_in_buf)++)
    {
        if (i == BUF_OF_64_ELEM)
                syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
        prog_stat->decl_vars[prog_stat->var_num].name[i] = buffer[*pos_in_buf];
        lex_stat->lexems[lex_stat->lex_size].name[i]     = buffer[*pos_in_buf];
    }
    prog_stat->decl_vars[prog_stat->var_num].name[i] = '\0';
    lex_stat->lexems[lex_stat->lex_size].name[i]     = '\0';

    if (*pos_in_buf == start_pos ||
        is_exist_name (prog_stat, lex_stat->lexems[lex_stat->lex_size].name, TYPE_VAR) != VAR_NOT_EXIST)
    {
        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    }
    prog_stat->decl_vars[prog_stat->var_num].line  = prog_stat->str_num;
    lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_VAR;
    lex_stat->lexems[lex_stat->lex_size].decloration = L_DECL;
    lex_stat->lexems[lex_stat->lex_size].num_of_var_func= prog_stat->var_num;
    prog_stat->var_num++;
    lex_stat->lex_size++;

    return 0;
}

int add_new_func (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    if (prog_stat->func_capacity <= prog_stat->func_num + 10) prog_stat_resize (prog_stat);
    if (lex_stat->lex_capacity   <= lex_stat->lex_size  + 10) lexems_resize    (lex_stat);

    int start_pos = *pos_in_buf;
    int i = 0;
    for (;(buffer[*pos_in_buf] >= 'a' && buffer[*pos_in_buf] <= 'z') ||
          (buffer[*pos_in_buf] >= 'A' && buffer[*pos_in_buf] <= 'Z'); i++, (*pos_in_buf)++)
    {
        if (i == BUF_OF_64_ELEM)
                syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
        prog_stat->decl_funcs[prog_stat->func_num].name[i] = buffer[*pos_in_buf];
        lex_stat->lexems[lex_stat->lex_size].name[i]       = buffer[*pos_in_buf];
    }
    prog_stat->decl_funcs[prog_stat->func_num].name[i] = '\0';
    lex_stat->lexems[lex_stat->lex_size].name[i]       = '\0';

    if (*pos_in_buf == start_pos ||
        is_exist_name (prog_stat, lex_stat->lexems[lex_stat->lex_size].name, TYPE_FUNC) != FUNC_NOT_EXIST)
    {
        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    }
    prog_stat->decl_funcs[prog_stat->func_num].line = prog_stat->str_num;
    lex_stat->lexems[lex_stat->lex_size].node_type  = TYPE_FUNC;
    lex_stat->lexems[lex_stat->lex_size].decloration= L_DECL;
    lex_stat->lexems[lex_stat->lex_size].num_of_var_func = prog_stat->func_num;

    printf ("%s\n", lex_stat->lexems[lex_stat->lex_size].name);
    if (strcmp (lex_stat->lexems[lex_stat->lex_size].name, "meow\0") == 0)
    {
        lex_stat->lexems[lex_stat->lex_size].num_of_var_func = 0;
    }
    prog_stat->func_num++;
    lex_stat->lex_size++;

    return 0;
}
