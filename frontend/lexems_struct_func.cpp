#include "frontend.h"

//=========================================LEXEMS_STRUCT_INIT=====================================================================

int lexems_init (lex_stat_t* lex_stat)
{
    MY_ASSERT (lex_stat != NULL)

    lex_stat->lex_capacity = 100;
    lex_stat->lex_size     = 0;

    lex_stat->lexems = (tree_node_t*) calloc (lex_stat->lex_capacity, sizeof (tree_node_t));
    MY_ASSERT (lex_stat->lexems != NULL)

    return 0;
}

int prog_stat_init (prog_data_t* prog_stat)
{
    MY_ASSERT (prog_stat != NULL)

    prog_stat->var_capacity  = 100;
    prog_stat->func_capacity = 10;
    prog_stat->var_num       = 0;

    prog_stat->decl_vars  = (prog_var_t*)  calloc (prog_stat->var_capacity,  sizeof (prog_var_t));
    prog_stat->decl_funcs = (prog_func_t*) calloc (prog_stat->func_capacity, sizeof (prog_func_t));
    MY_ASSERT (prog_stat->decl_vars != NULL && prog_stat->decl_funcs != NULL)

    return 0;
}

//=========================================LEXEMS_STRUCT_RESIZE=====================================================================

int lexems_resize (lex_stat_t* lex_stat)
{
    printf ("RESIZE%d %d\n", lex_stat->lex_capacity, lex_stat->lex_size);
    int prev_capacity =  lex_stat->lex_capacity;
    lex_stat->lex_capacity *= 2;
    tree_node_t* _lexems_resize  = (tree_node_t*) realloc (lex_stat->lexems, lex_stat->lex_capacity * sizeof (tree_node_t));

    for (int i = prev_capacity; i < lex_stat->lex_capacity; i++)
    {
        _lexems_resize[i].left  = NULL;
        _lexems_resize[i].right = NULL;
        _lexems_resize[i].value = 0;
        _lexems_resize[i].node_type = 0;
    }

    MY_ASSERT (_lexems_resize != NULL);
    lex_stat->lexems = _lexems_resize;

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

//=========================================LEXEMS_STRUCT_DTOR=====================================================================

int lex_dtor (lex_stat_t* lex_stat)
{
    free (lex_stat->lexems);
    lex_stat->lex_size = 0;

    return 0;
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
