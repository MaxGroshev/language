#include "frontend.h"
#define COMMENT

static int cur_lexem = 0;
tree_node_t* rec_descent (const char* file_dir)
{
    char* buffer = read_file (file_dir);

    prog_data_t prog_stat = {};
    prog_stat_init (&prog_stat);

    lex_stat_t lex_stat = {};
    lexems_init (&lex_stat);

    lexical_analysis (buffer, &prog_stat, &lex_stat);
    tree_node_t* ret_node = get_end (&lex_stat, &prog_stat);

    prog_data_dtor (buffer, &prog_stat);

    free (lex_stat.lexems); //make dtor

    //return ret_node;
    return NULL;
}

//------------------------------------REC_DESCENT_REALIZATION--------------------------------------------------------------

//#ifndef COMMENT

tree_node_t* get_end (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* tree_node = get_begin (lex_stat, prog_stat);

   // if (buffer[pos_in_file] != '$') syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG);
    return tree_node;
}

tree_node_t* get_operator (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_IF)
    {
        cur_lexem++;
        return get_if (lex_stat, prog_stat);
    }
    return get_assign (lex_stat, prog_stat);
}

tree_node_t* get_if (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* if_node   = &(lex_stat->lexems[cur_lexem - 1]);
    tree_node_t* cond_node = get_comp (lex_stat, prog_stat);

    if (lex_stat->lexems[cur_lexem].node_type == OP_THEN)
    {
        tree_delete (lex_stat->lexems[cur_lexem]);
        cur_lexem++;
        tree_node_t* r_exp = get_begin (buffer, prog_stat)
        tree_link_r (if_node, r_exp);
        tree_link_l (if_node, cond_node);
        return if_node;
    }
   // return get_operator (buffer, prog_stat); //remove
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG);

    return NULL;
}

tree_node_t* get_begin (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_BEGIN) // no node
    {
        tree_node_t* begin_node = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* area_node  = get_operator (lex_stat, prog_stat);
        tree_link_l (begin_node, area_node);

        if (lex_stat->lexems[cur_lexem].node_type != OP_END)
        {
            fprintf (stderr, "\033[91mNo end of expression\n \033[0m:\n");
            exit (-1);
        }
        tree_delete (lex_stat->lexems[cur_lexem]);
        cur_lexem++
        return begin_node;
    }
    else
    {
        return get_operator (lex_stat, prog_stat);
    }
    syntax_error (S_ERROR, NULL, CUR_POS_IN_PROG);

    return NULL;
}

tree_node_t* get_comp (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* sign_node = get_sign (lex_stat, prog_stat); //probably get_sign --> get_ident --> else (get_num))
    if ((lex_stat->lexems[cur_lexem].node_type == OP_COMP_EQ) ||
        (lex_stat->lexems[cur_lexem].node_type == OP_LESS)    ||
        (lex_stat->lexems[cur_lexem].node_type == OP_ABOVE)   ||
        (lex_stat->lexems[cur_lexem].node_type == OP_ABOVE_EQ)||
        (lex_stat->lexems[cur_lexem].node_type == OP_LESS_EQ))
    {
        tree_node_t* comp_node = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* assign_node = get_sign (buffer, prog_stat);
        tree_link_r (comp_node, assign_node);
        tree_link_l (comp_node, sign_node);
        return comp_node;
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG);
}

tree_node_t* get_assign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_ident (lex_stat, prog_stat);

    if (lex_stat->lexems[cur_lexem].node_type == OP_EQ) // add l_value notification
    {
        tree_node_t* operation = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_sign (lex_stat, prog_stat);
        if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N)
        {
            cur_lexem++;
            tree_link_r (operation, r_node);
            tree_link_l (operation, l_node);

            return make_gart_node (operation, get_begin (lex_stat, prog_stat));
        }
        syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG);
    }
    return l_node;
}

tree_node_t* get_ident (lex_stat_t* lex_stat, prog_data_t* prog_stat) //add multisymbol vars
{
    if (lex_stat->lexems[cur_lexem].node_type == TYPE_VAR)
    {
        cur_lexem++;
        return lex_stat->lexems[cur_lexem - 1];
    }
    else
    {
        return get_num  (lex_stat, prog_stat);
    }
}

tree_node_t* get_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    int is_neg_val = 0;
    if (lex_stat->lexems[cur_lexem].node_type == OP_SUB)
    {
        is_neg_val = 1;
        cur_lexem++;
    }
    tree_node_t* r_node = get_pm_sign (lex_stat, prog_stat);
    if (is_neg_val == 1) return tree_new_op_node (OP_MUL, tree_new_num_node (-1), r_node);
    return r_node;
}

tree_node_t* get_pm_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_md_sign (lex_stat, prog_stat);
    if (lex_stat->lexems[cur_lexem].node_type == OP_ADD || lex_stat->lexems[cur_lexem].node_type == OP_SUB)
    {
        tree_node_t* operation = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_md_sign (lex_stat, prog_stat);
        tree_link_r (operator, r_node);
        tree_link_l (operator, l_node);

        return operator;
    }
    return l_node;
}

tree_node_t* get_md_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_deg (lex_stat, prog_stat);
    while (lex_stat->lexems[cur_lexem].node_type == OP_MUL || lex_stat->lexems[cur_lexem].node_type == OP_DIV)
    {
        tree_node_t* operation = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_deg (lex_stat, prog_stat);
        tree_link_r (operator, r_node);
        tree_link_l (operator, l_node);

        return operator;
    }
    return l_node;
}

tree_node_t* get_deg (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_brac (lex_node, prog_stat);
    if (lex_stat->lexems[cur_lexem].node_type == OP_POW)
    {
        tree_node_t* operation = lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_brac (lex_stat, prog_stat);
        tree_node_t* r_node = get_deg  (lex_stat, prog_stat);
        tree_link_r (operator, r_node);
        tree_link_l (operator, l_node);

        return operator;
    }
    return l_node;
}

tree_node_t* get_brac (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_OPEN_BR)
    {
        tree_delete (lex_stat->lexems[cur_lexem].node_type);
        write_tree_logs (S_START_OF_BR_SEQ, NULL, CUR_POS_IN_PROG);
        cur_lexem++;
        tree_node_t* tree_node = get_sign (lex_stat, prog_stat);
        if (lex_stat->lexems[cur_lexem].node_type != OP_CLOSE_BR)
                syntax_error (S_NO_CLOSED_BRACKETS, NULL, CUR_POS_IN_PROG);

        tree_delete (lex_stat->lexems[cur_lexem].node_type);
        cur_lexem++;

        return tree_node;
    }
    else return get_ident (lex_stat, prog_stat);
}

tree_node_t* get_num (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == TYPE_NUM)
    {
        cur_lexem++;
        return lex_stat->lexems[cur_lexem - 1].node_type;
    }
    else syntax_error (S_NO_NUMBER, NULL, CUR_POS_IN_PROG);
}
//#endif
//----------------------------------------------------------------------------------------------------------------------

int prog_stat_init (prog_data_t* prog_stat)
{
    MY_ASSERT (prog_stat != NULL)

    prog_stat->var_capacity  = 10;
    prog_stat->func_capacity = 10;
    prog_stat->var_num       = 0;

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
    prog_stat->var_num       = 0;
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

        case S_ERROR:
            fprintf (stderr, "\033[91mName of syntax error wasn't detected. Check input: \033[0m\n");
            break;
    }
    // for (int i = 0; i < pos_in_file + 1; i++)
    // {
    //     fprintf (stderr, "%c", buffer[i]);
    // }
    fprintf (stderr, "\n");
    exit (-1);
}

