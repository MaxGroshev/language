//#include "frontend.h"
#include "rec_descent.h"

static int cur_lexem = 0;
tree_node_t* rec_descent (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* ret_node = get_gart_node (lex_stat, prog_stat);
    MY_ASSERT   (ret_node != NULL);

    return ret_node;
}

//===================================REC_DESCENT_REALIZATION===================================================================

tree_node_t* get_gart_node (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_PROG_END || lex_stat->lexems[cur_lexem].node_type == OP_END) return NULL;

    tree_node_t* l_node = get_func (lex_stat, prog_stat);
    printf ("-----%d\n", lex_stat->lexems[cur_lexem].node_type);
    if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N || lex_stat->lexems[cur_lexem].node_type == OP_END)
    {
        tree_node_t* garten_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_gart_node (lex_stat, prog_stat);
        if (l_node != NULL) tree_link_l (garten_node, l_node);
        if (r_node != NULL) tree_link_r (garten_node, r_node);

        return garten_node;
    }
    // else if (lex->stat[cur_lexem].node_type == OP_COMMA)
    // {
    //     cur_lexem++;
    //
    // }
    return l_node;
}

tree_node_t* get_func (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == TYPE_FUNC)
    {
        tree_node_t* func_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        printf ("empty_func %d\n", lex_stat->lexems[cur_lexem].node_type);
        tree_node_t* l_node = get_func (lex_stat, prog_stat);
//         if (lex_stat->lexems[cur_lexem].node_type == OP_COMMA);
//         {
//
//         }

        if (l_node != NULL) tree_link_l (func_node, l_node);
        if (lex_stat->lexems[cur_lexem].node_type == OP_BEGIN)
        {
            tree_node_t* r_node = get_begin (lex_stat, prog_stat);
            if (r_node != NULL) tree_link_r (func_node, r_node);
            return func_node;
        }
        printf ("empty_func %d\n", lex_stat->lexems[cur_lexem].node_type);
        // else if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N)

            return func_node;

    }
    return get_operator (lex_stat, prog_stat);
}

tree_node_t* get_operator (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_IF)
    {
        cur_lexem++;
        return get_if (lex_stat, prog_stat);
    }
    else if (lex_stat->lexems[cur_lexem].node_type == OP_RETURN)
    {
        tree_node_t* return_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node      = get_func (lex_stat, prog_stat);
        tree_link_r (return_node, r_node);
        return return_node;
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
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);

    return NULL;
}

tree_node_t* get_begin (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    if (lex_stat->lexems[cur_lexem].node_type == OP_BEGIN)
    {
        tree_node_t* begin_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* area_node = get_gart_node (lex_stat, prog_stat);
        tree_link_l (begin_node, area_node);

        printf ("-----%d------\n", lex_stat->lexems[cur_lexem].node_type);
        printf ("-----%d------\n", lex_stat->lexems[cur_lexem+1].node_type);
        if (lex_stat->lexems[cur_lexem].node_type == OP_END)
        {
            printf ("I am here to end! %d\n", cur_lexem);
            return begin_node;
        }
    }
    syntax_error (S_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);

    return NULL;
}

tree_node_t* get_comp (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* sign_node = get_comma (lex_stat, prog_stat);

    if ((lex_stat->lexems[cur_lexem].node_type == OP_COMP_EQ) ||
        (lex_stat->lexems[cur_lexem].node_type == OP_LESS)    ||
        (lex_stat->lexems[cur_lexem].node_type == OP_ABOVE)   ||
        (lex_stat->lexems[cur_lexem].node_type == OP_ABOVE_EQ)||
        (lex_stat->lexems[cur_lexem].node_type == OP_LESS_EQ))
    {
        tree_node_t* comp_node = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* assign_node = get_comma (lex_stat, prog_stat);
        tree_link_r (comp_node, assign_node);
        tree_link_l (comp_node, sign_node);
        return comp_node;
    }
    syntax_error (S_UNREC_SYNTAX_ERROR, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    return NULL;
}

tree_node_t* get_assign (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* l_node = get_brac (lex_stat, prog_stat);

    if (lex_stat->lexems[cur_lexem].node_type == OP_EQ)
    {
       // printf ("%s\n", lex_stat->lexems[cur_lexem - 1].name );
        tree_node_t* operation = &lex_stat->lexems[cur_lexem];
        cur_lexem++;
        tree_node_t* r_node = get_comma (lex_stat, prog_stat);
        if (lex_stat->lexems[cur_lexem].node_type == OP_GART_N)
        {
            tree_link_r (operation, r_node);
            tree_link_l (operation, l_node);

            return operation;
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

tree_node_t* get_comma (lex_stat_t* lex_stat, prog_data_t* prog_stat)
{
    tree_node_t* expression = get_pm_sign (lex_stat, prog_stat);

    if (lex_stat->lexems[cur_lexem].node_type == OP_COMMA)
    {
        cur_lexem++;
        tree_node_t* l_node = get_comma (lex_stat, prog_stat);
        tree_link_l (expression, l_node);
        return expression;
    }
    return expression;
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
    tree_node_t* l_node = get_func (lex_stat, prog_stat);
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
    printf ("%d\n", lex_stat->lexems[cur_lexem].node_type);
    if (lex_stat->lexems[cur_lexem].node_type == OP_OPEN_BR)
    {
        cur_lexem++;
        tree_node_t* tree_node = get_comma (lex_stat, prog_stat);
        printf ("BRACKET%d\n", lex_stat->lexems[cur_lexem].node_type);
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
    else if (lex_stat->lexems[cur_lexem - 2].node_type == TYPE_FUNC &&
             lex_stat->lexems[cur_lexem - 1].node_type == OP_OPEN_BR &&
             lex_stat->lexems[cur_lexem].node_type     == OP_CLOSE_BR)
    {
        printf ("here\n");
        return NULL;
    }

    syntax_error (S_NO_NUMBER, NULL, CUR_POS_IN_PROG, lex_stat->lexems[cur_lexem].node_type);
    return NULL;
}

//==========================================ERROR_PROCESSING============================================================

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

