#include "frontend.h"
#define COMMENT

static int cur_lexem = 0;
tree_node_t* rec_descent (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* ret_node = get_end (lex_stat, prog_stat);
    MY_ASSERT   (ret_node != NULL);

    return ret_node;
}

//------------------------------------REC_DESCENT_REALIZATION--------------------------------------------------------------

//#ifndef COMMENT

tree_node_t* get_end (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* tree_node = get_gart_node (lex_stat, prog_stat);

    //if (buffer[pos_in_file] != '$') syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    return tree_node;
}

tree_node_t* get_gart_node (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_PROG_END || lex_stat->lexems[cur_lexem].node_type == OP_END) return NULL;

    tree_node_t* l_node = get_operator (lex_stat, prog_stat);
    printf ("-----%d\n", lex_stat->lexems[cur_lexem].node_type);
    if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N)
    {
        tree_node_t* garten_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_gart_node (lex_stat, prog_stat);
        tree_link_l (garten_node, l_node);
        if (r_node != NULL) tree_link_r (garten_node, r_node);

        return garten_node;
    }
    return l_node;
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
        cur_lexem++;
        tree_node_t* r_exp = get_begin (lex_stat, prog_stat);
        tree_link_r (if_node, r_exp);
        tree_link_l (if_node, cond_node);
        return if_node;
    }
   // return get_operator (buffer, prog_stat); //remove
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);

    return NULL;
}

tree_node_t* get_begin (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_BEGIN) // no node
    {
        tree_node_t* begin_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* area_node = get_gart_node (lex_stat, prog_stat);
        tree_link_l (begin_node, area_node);

            printf ("-----%d------\n", lex_stat->lexems[cur_lexem].node_type);

        if (lex_stat->lexems[cur_lexem].node_type == OP_END)
        {
            //cur_lexem++;
            return begin_node;
            // fprintf (stderr, "\033[91mNo end of expression\n \033[0m:\n");
            // exit (-1);
        }
        cur_lexem++;
        return begin_node;
    }
    else
    {
        return get_operator (lex_stat, prog_stat);
    }
    syntax_error (S_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);

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
        tree_node_t* comp_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* assign_node = get_sign (lex_stat, prog_stat);
        tree_link_r (comp_node, assign_node);
        tree_link_l (comp_node, sign_node);
        return comp_node;
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    return NULL;
}

tree_node_t* get_assign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_ident (lex_stat, prog_stat);

    if (lex_stat->lexems[cur_lexem].node_type == OP_EQ)
    {
        tree_node_t* operation = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_sign (lex_stat, prog_stat);
        if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N)
        {
            tree_link_r (operation, r_node);
            tree_link_l (operation, l_node);

            return operation;//make_gart_node (operation, get_begin (lex_stat, prog_stat));
        }
        syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    }
    return l_node;
}

tree_node_t* get_ident (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == TYPE_VAR)
    {
        cur_lexem++;
        return &lex_stat->lexems[cur_lexem - 1];
    }
    else
    {
        return get_num  (lex_stat, prog_stat);
    }
}

tree_node_t* get_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    int is_neg_val = 0;
    if (lex_stat->lexems[cur_lexem].value == -1 && lex_stat->lexems[cur_lexem + 1].node_type == OP_MUL)
    {
        tree_node_t* l_node    = &lex_stat->lexems[cur_lexem];
        tree_node_t* operation = &lex_stat->lexems[cur_lexem + 1];
        cur_lexem += 2;
        tree_node_t* r_node = get_pm_sign (lex_stat, prog_stat);
        MY_ASSERT (r_node != NULL);
        tree_link_r (operation, r_node);
        tree_link_l (operation, l_node);

        return operation;
    }
    tree_node_t* r_node = get_pm_sign (lex_stat, prog_stat);
    return r_node;
}

tree_node_t* get_pm_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_md_sign (lex_stat, prog_stat);
    if (lex_stat->lexems[cur_lexem].node_type == OP_ADD || lex_stat->lexems[cur_lexem].node_type == OP_SUB)
    {
        tree_node_t* operation = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_md_sign (lex_stat, prog_stat);
        tree_link_r (operation, r_node);
        tree_link_l (operation, l_node);

        return operation;
    }
    return l_node;
}

tree_node_t* get_md_sign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_deg (lex_stat, prog_stat);
    while (lex_stat->lexems[cur_lexem].node_type == OP_MUL || lex_stat->lexems[cur_lexem].node_type == OP_DIV)
    {
        tree_node_t* operation = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_deg (lex_stat, prog_stat);
        tree_link_r (operation, r_node);
        tree_link_l (operation, l_node);

        return operation;
    }
    return l_node;
}

tree_node_t* get_deg (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_brac (lex_stat, prog_stat);
    if (lex_stat->lexems[cur_lexem].node_type == OP_POW)
    {
        tree_node_t* operation = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_deg  (lex_stat, prog_stat);
        tree_link_r (operation, r_node);
        tree_link_l (operation, l_node);

        return operation;
    }
    return l_node;
}

tree_node_t* get_brac (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_OPEN_BR)
    {
        cur_lexem++;
        tree_node_t* tree_node = get_sign (lex_stat, prog_stat);
        if (lex_stat->lexems[cur_lexem].node_type != OP_CLOSE_BR)
                syntax_error (S_NO_CLOSED_BRACKETS, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);

        cur_lexem++;

        return tree_node;
    }
    return get_ident (lex_stat, prog_stat);
}

tree_node_t* get_num (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == TYPE_NUM)
    {
        cur_lexem++;
        return &lex_stat->lexems[cur_lexem - 1];
    }
    syntax_error (S_NO_NUMBER, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    return NULL;
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

void syntax_error (int num_of_error, const char* buffer, const char* file_name, const char* func_name, int num_of_line, int node_code)
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
    fprintf (stderr, "%d", node_code);
    fprintf (stderr, "\n");
    exit (-1);
}

