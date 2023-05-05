#include "frontend.h"

static int pos_in_file = 0;
tree_node_t* rec_descent (const char* file_dir)
{
    char* buffer = read_file (file_dir);

    prog_data_t prog_stat = {};
    prog_stat_init (&prog_stat);

    tree_node_t* ret_node = get_end (buffer, &prog_stat);

    prog_data_dtor (buffer, &prog_stat);
    return ret_node;
}

//------------------------------------REC_DESCENT_REALIZATION--------------------------------------------------------------

tree_node_t* get_end (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* tree_node = get_begin (buffer, prog_stat);

    if (buffer[pos_in_file] != '$') syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    return tree_node;
}

tree_node_t* get_operator (const char* buffer, prog_data_t* prog_stat)
{
    if (strncomp (buffer + pos_in_file, "if", strlen ("if"), STR_SKIP_SPACE))
    {
        tree_node_t* if_node =  get_if (buffer, prog_stat);
        if (strncomp (buffer + pos_in_file, ";\n", strlen (";\n"), STR_SKIP_SPACE))
        {
            tree_node_t* r_exp = get_operator (buffer, prog_stat);
            tree_link_r (if_node->right, r_exp);
        }
        return if_node;
    }
    tree_node_t* assign_node = get_assign (buffer, prog_stat);
    if (strncomp (buffer + pos_in_file, ";\n", strlen (";\n"), STR_SKIP_SPACE))
    {
        tree_node_t* r_exp = get_operator (buffer, prog_stat);
        tree_link_r (assign_node->right, r_exp);
    }
    return assign_node;
}

tree_node_t* get_if (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* cond_node = get_comp (buffer, prog_stat);

    if (strncomp (buffer + pos_in_file, "then\n", strlen ("then\n"), STR_SKIP_SPACE))
    {
        tree_node_t* expr_node = get_begin (buffer, prog_stat);
        return tree_new_op_node (OP_IF, cond_node, expr_node); // not return go to get right node (assigment or operator)
    }
   // return get_operator (buffer, prog_stat); //remove
    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
}

tree_node_t* get_begin (const char* buffer, prog_data_t* prog_stat)
{
    if (strncomp (buffer + pos_in_file, "begin:\n", strlen ("begin:\n"), STR_SKIP_SPACE))
    {
        tree_node_t* l_exp   = get_operator (buffer, prog_stat);
        //tree_node_t* r_exp   = get_operator (buffer, prog_stat);
        //return make_gart_node (l_exp, r_exp);
        return l_exp;
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
}

tree_node_t* get_comp (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* tree_node = get_sign (buffer, prog_stat); //probably get_sign --> get_ident --> else (get_num))
    if (strncomp (buffer + pos_in_file, "==", strlen ("=="), STR_SKIP_SPACE)) // add l_value notification
    {
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_COMP_EQ, tree_node, assign_node);
    }
    else if (strncomp (buffer + pos_in_file, "<=", strlen ("<="), STR_SKIP_SPACE)) // add l_value notification
    {
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_LESS_EQ, tree_node, assign_node);
    }
    else if (strncomp (buffer + pos_in_file, ">=", strlen (">="), STR_SKIP_SPACE))
    {
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_ABOVE_EQ, tree_node, assign_node); //add choice about skipping shifts
    }
    else if (strncomp (buffer + pos_in_file, ">", strlen (">"), STR_SKIP_SPACE)) // add l_value notification
    {
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_ABOVE, tree_node, assign_node);
    }
    else if (strncomp (buffer + pos_in_file, "<", strlen ("<"), STR_SKIP_SPACE)) // add l_value notification
    {
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_LESS, tree_node, assign_node);
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    return tree_node;
}

tree_node_t* get_assign (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* tree_node = get_ident (buffer, prog_stat);
    if (buffer[pos_in_file] == '=' && tree_node->node_type == TYPE_VAR) // add l_value notification
    {
        pos_in_file++;
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        return tree_new_op_node (OP_EQ, tree_node, assign_node);

        syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    }
    return tree_node;
}

tree_node_t* get_area_end (const char* buffer, prog_data_t* prog_stat)
{

    printf ("want to finish\n");
    if (strncomp (buffer + pos_in_file, "end!\n", strlen ("end!\n"), STR_SKIP_SPACE))
    {
        return make_gart_node ();
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
}

tree_node_t* get_ident (const char* buffer, prog_data_t* prog_stat) //add multisymbol vars
{
    if (strncomp (buffer + pos_in_file, "var", strlen ("var"), STR_SKIP_SPACE))
    {
        if ((buffer[pos_in_file] >= 'a' && buffer[pos_in_file] <= 'z') ||
            (buffer[pos_in_file] >= 'A' && buffer[pos_in_file] <= 'Z'))
        {
            char var = buffer[pos_in_file];
            add_new_var (var, prog_stat);
            pos_in_file++;
            return tree_new_var_node (TYPE_VAR, var);
        }
        syntax_error (S_NO_MUL_OR_DIV_OP, buffer, CUR_POS_IN_PROG);
    }

    if (((buffer[pos_in_file] >= 'a' && buffer[pos_in_file] <= 'z') ||
        (buffer[pos_in_file] >= 'A' && buffer[pos_in_file] <= 'Z')) && buffer[pos_in_file] != 'e')

    {
        char var = buffer[pos_in_file];
        printf ("%c -- char\n", var);
        pos_in_file++;
        return tree_new_var_node (TYPE_VAR, var);
    }
    else
    {
       // printf ("here\n");
        return get_num  (buffer, prog_stat);
    }
}

tree_node_t* get_sign (const char* buffer, prog_data_t* prog_stat)
{
    int is_neg_val = 0;
    if (buffer[pos_in_file] == '-')
    {
        is_neg_val = 1;
        pos_in_file++;
    }
    tree_node_t* r_node = get_pm_sign (buffer, prog_stat);
    if (is_neg_val == 1) return tree_new_op_node (OP_MUL, tree_new_num_node (-1), r_node);
    return r_node;
}

tree_node_t* get_pm_sign (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_md_sign (buffer, prog_stat);

    while (buffer[pos_in_file] == '+' || buffer[pos_in_file] == '-')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_md_sign (buffer, prog_stat);
        if      (op == '+') l_node = tree_new_op_node (OP_ADD, l_node, r_node);
        else if (op == '-') l_node = tree_new_op_node (OP_SUB, l_node, r_node);
    }
    return l_node;
}

tree_node_t* get_md_sign (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_deg (buffer, prog_stat);
    while (buffer[pos_in_file] == '*' || buffer[pos_in_file] == '/')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_deg (buffer, prog_stat);
        if      (op == '*')  l_node = tree_new_op_node (OP_MUL, l_node, r_node);
        else if (op == '/')  l_node = tree_new_op_node (OP_DIV, l_node, r_node);
        else    syntax_error (S_NO_MUL_OR_DIV_OP, buffer, CUR_POS_IN_PROG);
    }
    return l_node;
}

tree_node_t* get_deg (const char* buffer, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_brac(buffer, prog_stat);
    while (buffer[pos_in_file] == '^')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_brac (buffer, prog_stat);
        if      (op == '^')  l_node = tree_new_op_node (OP_POW, l_node, r_node);// make error
        else    syntax_error (S_NO_MUL_OR_DIV_OP, buffer, CUR_POS_IN_PROG);
    }
    return l_node;
}

tree_node_t* get_brac (const char* buffer, prog_data_t* prog_stat)
{
    if ((buffer[pos_in_file] == '(') || (buffer[pos_in_file] == '['))
    {
        write_tree_logs (S_START_OF_BR_SEQ, NULL, CUR_POS_IN_PROG);
        pos_in_file++;
        tree_node_t* tree_node = get_sign (buffer, prog_stat);
        if ((buffer[pos_in_file] != ')') && (buffer[pos_in_file] != ']')) syntax_error (S_NO_CLOSED_BRACKETS, buffer, CUR_POS_IN_PROG);
        pos_in_file++;
        return tree_node;
    }
    else return get_ident (buffer, prog_stat);
}

tree_node_t* get_num (const char* buffer, prog_data_t* prog_stat)
{
    int val = 0;
    int start_pos = pos_in_file;
    while (buffer[pos_in_file] >= '0' && buffer[pos_in_file] <= '9')
    {
        val = val * 10 + buffer[pos_in_file] - '0';
        pos_in_file++;
    }
    if   (start_pos < pos_in_file) return tree_new_num_node (val);
    else return get_area_end (buffer, prog_stat);
}


//-----------------------------------------------------------------------------------------------------------------

int add_new_var (char var_name, prog_data_t* prog_stat)
{
    if (prog_stat->var_capacity >= prog_stat->var_num + 2)
    {
        prog_stat_resize (prog_stat);
    }
    prog_stat->decl_vars[prog_stat->var_num].name = var_name;
    prog_stat->var_num++;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int prog_stat_init (prog_data_t* prog_stat)
{
    MY_ASSERT (prog_stat != NULL)

    prog_stat->var_capacity  = 10;
    prog_stat->func_capacity = 10;

    prog_stat->decl_vars  = (prog_var_t*)  calloc (prog_stat->var_capacity,  sizeof (prog_var_t));
    prog_stat->decl_funcs = (prog_func_t*) calloc (prog_stat->func_capacity, sizeof (prog_func_t));
    MY_ASSERT (prog_stat->decl_vars != NULL && prog_stat->decl_funcs != NULL)

    return 0;
}

int prog_stat_resize (prog_data_t* prog_stat)
{
    MY_ASSERT (prog_stat != NULL)

    if (prog_stat->var_capacity >= prog_stat->var_num + 2)
    {
        prog_stat->var_capacity *= 2;
        prog_var_t* decl_vars_resize  = (prog_var_t*) realloc (prog_stat->decl_vars, prog_stat->var_capacity * sizeof (prog_var_t));

        MY_ASSERT (decl_vars_resize != NULL);
        prog_stat->decl_vars = decl_vars_resize;
    }
    if (prog_stat->func_capacity >= prog_stat->func_num + 2)
    {
        prog_stat->var_capacity *= 2;
        prog_func_t* decl_funcs_resize  = (prog_func_t*) realloc (prog_stat->decl_funcs, prog_stat->func_capacity * sizeof (prog_func_t));

        MY_ASSERT (decl_funcs_resize != NULL);
        prog_stat->decl_funcs = decl_funcs_resize;
    }

    return 0;
}

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

int prog_data_dtor (char* buffer, prog_data_t* prog_stat)
{
    free (buffer);
    free (prog_stat->decl_vars);
    free (prog_stat->decl_funcs);

    prog_stat->var_capacity  = 0;
    prog_stat->func_capacity = 0;
    prog_stat->str_num       = 0;
    prog_stat->func_num      = 0;
    prog_stat->str_num       = 0;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------

void syntax_error (int num_of_error, const char* buffer, const char* file_name, const char* func_name, int num_of_line)
{
    fprintf (stderr, "\033[91mSyntaxERROR  \033[0m: FILE: %s, FUNCTION: %s, LINE: %d\n", file_name, func_name, num_of_line);
    switch (num_of_error)
    {
        case S_NO_CLOSED_BRACKETS:
            fprintf (stderr, "\033[91mNo closing bracket \033[0m:\n"); // Improve this_func and move to another file
            break;

        case S_NO_NUMBER:
            fprintf (stderr, "\033[91mWas expected num_value  \033[0m:\n");
            break;

        case S_NO_MUL_OR_DIV_OP:
            fprintf (stderr, "\033[91mWas expected div or mul operation \033[0m:\n");
            break;

        case S_UNREC_SYNTAX_ERROR:
            fprintf (stderr, "\033[91mName of syntax error wasn't detected. Check input: \033[0m\n");
            break;
    }
    for (int i = 0; i < pos_in_file + 1; i++)
    {
        fprintf (stderr, "%c", buffer[i]);
    }
    fprintf (stderr, "\n");
    exit (-1);
}

//-------------------------------STRING FUNC: compare num_of_elem of str1 and str2---------------------------------------------------------------------------------

int strncomp (const char* str1, const char* str2, size_t num_of_elem, int skip_space)
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
        int begin_pos = pos_in_file;
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
                pos_in_file = begin_pos;
                return 0;
            }
        }
        while (*(str1 + read_elem) == 32)
        {
            read_elem++;
        }
        pos_in_file = begin_pos + read_elem;
        return 1;
    }
    return 0;
}
