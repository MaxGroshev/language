#include "frontend.h"

int lexical_analysis (char* buffer, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    for (int pos_in_buf = 0; buffer[pos_in_buf] != '\0';)
    {
        if (lex_stat->lex_capacity <= lex_stat->lex_size + 2) lexems_resize (lex_stat);
        if (buffer[pos_in_buf] == '$')
        {
            lex_stat->lexems[lex_stat->lex_size].node_type = OP_GART_N;
            lex_stat->lex_size++;
            break;
        }
        while (buffer[pos_in_buf]  == 32) pos_in_buf++;
        if    (buffer[pos_in_buf] == '\n' || buffer[pos_in_buf] == '\r' || buffer[pos_in_buf] == '\t')
        {
            prog_stat->str_num++;
            pos_in_buf++;
            continue;
        }

        is_this_op ("end!",   OP_END, buffer, &pos_in_buf, lex_stat);
        is_this_op ("begin:", OP_BEGIN, buffer, &pos_in_buf, lex_stat);
        is_this_op ("then",   OP_THEN, buffer, &pos_in_buf, lex_stat);

        if (l_strncomp (buffer + pos_in_buf, "var", strlen ("var"), STR_SKIP_SPACE, &pos_in_buf))
        {
            int start_pos = pos_in_buf;
            char var_name[BUF_OF_64_ELEM];

            int i = 0;
            for (; (buffer[pos_in_buf] >= 'a' && buffer[pos_in_buf] <= 'z') ||
                   (buffer[pos_in_buf] >= 'A' && buffer[pos_in_buf] <= 'Z'); i++, pos_in_buf++)
            {
                if (i == BUF_OF_64_ELEM)
                        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
                var_name[i] = buffer[pos_in_buf];
            }
            var_name[i] = '\0';

            if (pos_in_buf == start_pos) syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
            else
            {
                add_new_var (var_name, prog_stat, lex_stat);
            }
            //printf ("%s 11\n", lex_stat->lexems[lex_stat->lex_size - 1].name);
        }
        is_this_op ("=", OP_EQ, buffer, &pos_in_buf, lex_stat);
        if (buffer[pos_in_buf] >= '0' && buffer[pos_in_buf] <= '9')
        {
            lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_NUM;
            lex_stat->lexems[lex_stat->lex_size].value     = my_strtod (buffer + pos_in_buf, &pos_in_buf);
            lex_stat->lex_size++;
            printf ("%lg\n", lex_stat->lexems[lex_stat->lex_size - 1].value);
        }

        if (is_exist_var (buffer, pos_in_buf, prog_stat))
        {
            char var_name[BUF_OF_64_ELEM];
            for (int i = 0; (buffer[pos_in_buf] >= 'a' && buffer[pos_in_buf] <= 'z') ||
                            (buffer[pos_in_buf] >= 'A' && buffer[pos_in_buf] <= 'Z'); i++, pos_in_buf++)
            {
                if (i == BUF_OF_64_ELEM)
                        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
                var_name[i] = buffer[pos_in_buf];
            }
            lex_stat->lexems[lex_stat->lex_size].name      = var_name;
            lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_VAR;
            lex_stat->lex_size++;
        }

        is_this_op (";",  OP_GART_N, buffer, &pos_in_buf, lex_stat);
        is_this_op ("if", OP_IF,     buffer, &pos_in_buf, lex_stat); // make end node
        is_this_op ("(",  OP_OPEN_BR,  buffer, &pos_in_buf, lex_stat);
        is_this_op (")",  OP_CLOSE_BR, buffer, &pos_in_buf, lex_stat);

        is_this_op ("==", OP_COMP_EQ,  buffer, &pos_in_buf, lex_stat);
        is_this_op (">=", OP_ABOVE_EQ, buffer, &pos_in_buf, lex_stat);
        is_this_op ("<=", OP_LESS_EQ,  buffer, &pos_in_buf, lex_stat);
        is_this_op (">",  OP_ABOVE,    buffer, &pos_in_buf, lex_stat);
        is_this_op ("<",  OP_LESS,     buffer, &pos_in_buf, lex_stat);

        is_this_op ("+",  OP_ADD,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("-",  OP_SUB,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("*",  OP_MUL,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("/",  OP_DIV,      buffer, &pos_in_buf, lex_stat);
        is_this_op ("^",  OP_POW,      buffer, &pos_in_buf, lex_stat);
    }
    return 0;
}

int is_this_op (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat)
{
    if (lex_stat->lex_capacity <= lex_stat->lex_size + 2) lexems_resize (lex_stat);

    if (l_strncomp (buffer + *pos_in_buf, str, strlen (str), STR_SKIP_SPACE, pos_in_buf))
    {
        lex_stat->lexems[lex_stat->lex_size].node_type = code_of_op;
        lex_stat->lex_size++;
        // if (code_of_op == OP_IF || code_of_op == OP_EQ)
        // {
        //     lex_stat->lexems[lex_stat->lex_size].node_type = OP_GART_N;
        //     lex_stat->lex_size++;
        // }
        printf ("%c 11\n", lex_stat->lexems[lex_stat->lex_size - 1].node_type);
        return 1;
    }
    return 0;
}

int is_exist_var (const char* buffer, int pos_in_buf, prog_data_t* prog_stat)
{
    if ((buffer[pos_in_buf] >= 'a' && buffer[pos_in_buf] <= 'z') ||
        (buffer[pos_in_buf] >= 'A' && buffer[pos_in_buf] <= 'Z'))
    {
        char var_name[BUF_OF_64_ELEM];
        int i = 0;
        for (; (buffer[pos_in_buf] >= 'a' && buffer[pos_in_buf] <= 'z') ||
                        (buffer[pos_in_buf] >= 'A' && buffer[pos_in_buf] <= 'Z'); i++, pos_in_buf++)
        {
            printf ("%d) %c\n", i, buffer[pos_in_buf]);
            if (i == BUF_OF_64_ELEM)
                    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
            var_name[i] = buffer[pos_in_buf];
        }
        var_name[i] = '\0';
        for (int i = 0; i < prog_stat->var_num; i++)
        {
            MY_ASSERT (prog_stat->decl_vars != NULL)
            if (strcmp (var_name, prog_stat->decl_vars[i].name) == 0)
            {
                printf ("COMPARED: %s\n", prog_stat->decl_vars[i].name);
                return 1;
            }
        }
        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    }
    return 0;
}

int lexems_init (lex_stat_t* lex_stat)
{
    MY_ASSERT (lex_stat != NULL)

    lex_stat->lex_capacity = 10;
    lex_stat->lex_size     = 0;

    lex_stat->lexems = (tree_node_t*) calloc (lex_stat->lex_capacity, sizeof (tree_node_t));
    MY_ASSERT (lex_stat->lexems != NULL)

    return 0;
}

int lexems_resize (lex_stat_t* lex_stat)
{
    printf ("%d\n", lex_stat->lex_capacity);
    lex_stat->lex_capacity *= 2;
    tree_node_t* _lexems_resize  = (tree_node_t*) realloc (lex_stat->lexems, lex_stat->lex_capacity * sizeof (tree_node_t));

    MY_ASSERT (_lexems_resize != NULL);
    lex_stat->lexems = _lexems_resize;

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------

int add_new_var (char* var_name, prog_data_t* prog_stat, lex_stat_t* lex_stat)
{
    if (prog_stat->var_capacity <= prog_stat->var_num + 2) prog_stat_resize (prog_stat);
    if (lex_stat->lex_capacity  <= lex_stat->lex_size + 2) lexems_resize    (lex_stat);
    prog_stat->decl_vars[prog_stat->var_num].name = var_name;

    prog_stat->decl_vars[prog_stat->var_num].line = prog_stat->str_num;
    //prog_stat->decl_vars[prog_stat->var_num].func_name = str_num; //add
    prog_stat->var_num++;

    lex_stat->lexems[lex_stat->lex_size].name      = var_name;
    lex_stat->lexems[lex_stat->lex_size].node_type = TYPE_VAR;
    lex_stat->lex_size++;

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

int l_strncomp (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf)
{
    if (skip_space == STR_NOT_SKIP_SPACE)
    {
        for (int i = 0; i < num_of_elem; i++)
        {
            if ((str1 + i == NULL) || (str2 + i == NULL) || (*(str1 + i) != *(str2 + i)))
            {
                return 0;
            }
        }
        return 1;
    }
    else if (skip_space == STR_SKIP_SPACE)
    {
        int begin_pos = *pos_in_buf;
        int read_elem = 0;
        for (int comp_elem = 0; comp_elem < num_of_elem; read_elem++, comp_elem++)
        {
            if ((*(str1 + read_elem) == 32 || *(str1 + read_elem) == '\n') && comp_elem == 0)
            {
                comp_elem--;
                continue;
            }
            printf ("%c--%c %d\n", *(str1 + read_elem),  *(str2 + comp_elem), comp_elem);
            if ((str1 + read_elem == NULL) || (str2 + comp_elem == NULL) || (*(str1 + read_elem) != *(str2 + comp_elem)))
            {
                *pos_in_buf = begin_pos;
                return 0;
            }
        }
        while (*(str1 + read_elem) == 32)
        {
            read_elem++;
        }
        *pos_in_buf = begin_pos + read_elem;
        return 1;
    }
    return 0;
}

double my_strtod (const char* str, int* pos_in_buf)
{
    int point_flag = 0;
    char value_in_str[BUF_OF_64_ELEM];
    int i = 0;
    for (; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++)
    {
        if ((str[0] == '0' && str[1] != '.') || (i == BUF_OF_64_ELEM) || (str[i] == '.' && point_flag != 0))
                syntax_error (S_NO_CLOSED_BRACKETS, str, CUR_POS_IN_PROG); // not str to error

        if (str[i] == '.') point_flag = 1;
        value_in_str[i] = str[i];
    }
    value_in_str[i] = '\0';
    *pos_in_buf += i;

    double value = 0;
    sscanf (value_in_str, "%lg", &value);
    printf ("---%lg---\n", value);

    return value;
}
