#include "backend.h"

int write_asm_code (tree_node_t* prog_tree, FILE* prog_file, prog_data_t* prog_data)
{
    if (prog_tree->node_type == TYPE_FUNC && prog_tree->decloration == L_DECL)
    {
        fprintf (prog_file, ":%d\r\n", prog_tree->num_of_var_func);
        if (prog_tree->left != NULL)
        {
            prog_tree = func_decl_def (prog_tree, prog_file, prog_data);
        }
    }
    if (prog_tree->left != NULL)
    {
        write_asm_code (prog_tree->left, prog_file, prog_data);
    }
    if (prog_tree->right != NULL)
    {
        write_asm_code (prog_tree->right, prog_file, prog_data);
    }

    switch (prog_tree->node_type)
    {
        case OP_EQ:
            fprintf (prog_file, "pop  [%d]\r\npop  ax\r\n", prog_tree->left->num_of_var_func);
            return 1;
        case TYPE_NUM:
            fprintf (prog_file, "push %lg\r\n", prog_tree->value);
            return 1;
        case TYPE_VAR:
            fprintf (prog_file, "push [%d]\r\n", prog_tree->num_of_var_func);
            return 1;

        case OP_IF:
            if (prog_data->label < prog_data->cond_depth) prog_data->label = prog_data->cond_depth;
            printf ("FUCK %d\n", prog_data->cond_depth);
            prog_data->cond_depth--;
            fprintf (prog_file, ":%d\r\n", prog_data->cond_depth);

            return 1;
        //case OP_COMMA:      fprintf (prog_file, ",");           break; // do not know
        case OP_RETURN:     fprintf (prog_file, "ret\r\n"); break;
    }
    if      (math_opr_def (prog_tree->node_type, prog_file)) return 1;
    else if (func_def     (prog_tree, prog_file, prog_data)) return 1;
    else if (cond_jmp_def (prog_tree->node_type, prog_file, prog_data))
    {
        prog_data->cond_depth++;
        return 1;
    }
    return 0;
}

//========================================================================================

int math_opr_def (int node_type, FILE* prog_file)
{
    switch (node_type)
    {
        case OP_ADD:        fprintf (prog_file, "add\r\n");      return 1;
        case OP_SUB:        fprintf (prog_file, "sub\r\n");      return 1;
        case OP_MUL:        fprintf (prog_file, "mul\r\n");      return 1;
        case OP_DIV:        fprintf (prog_file, "div\r\n");      return 1;
//      case OP_POW:        fprintf (prog_file, "^");            return 1;
    }

    return 0;
}

tree_node_t* func_decl_def (tree_node_t* prog_tree, FILE* prog_file, prog_data_t* prog_data)
{
    // if (prog_tree->right != NULL)
    // {
    //     write_asm_code (prog_tree->right, prog_file, prog_data);
    // }
    // if (prog_tree->left != NULL)
    // {
    //     write_asm_code (prog_tree->left, prog_file, prog_data);
    // }
    fprintf (prog_file, "pop  [%d]\r\n", prog_tree->left->num_of_var_func);

    return prog_tree->right;
}


int func_def (tree_node_t* prog_tree, FILE* prog_file, prog_data_t* prog_data)
{
    if (prog_tree->node_type == TYPE_FUNC)
    {
        if      (prog_tree->value == LIB_PRINT)   fprintf (prog_file, "out\r\n");
        else if (prog_tree->value == LIB_SQR)     fprintf (prog_file, "sqrt\r\n");
        else if (prog_tree->value == LIB_WRITELN)
        {
            fprintf (prog_file, "in\r\n");
            fprintf (prog_file, "pop [%d]\r\n", prog_tree->left->num_of_var_func);
        }
        else if (prog_tree->decloration == L_MENTION)
        {
            fprintf (prog_file, "call :%d\r\n", prog_tree->num_of_var_func);
            prog_data->label++;
        }
        return 1;
    }

    return 0;
}

int cond_jmp_def (int node_type, FILE* prog_file, prog_data_t* prog_data)
{
    int ret_val = 0;
    switch (node_type)
    {
        case OP_LESS:       fprintf (prog_file, "jb  :"); ret_val = 1; break;
        case OP_ABOVE:      fprintf (prog_file, "ja  :"); ret_val = 1; break;
        case OP_ABOVE_EQ:   fprintf (prog_file, "jae :"); ret_val = 1; break;
        case OP_LESS_EQ:    fprintf (prog_file, "jbe :"); ret_val = 1; break;
        case OP_COMP_EQ:    fprintf (prog_file, "jne :"); ret_val = 1; break;
        case OP_N_COMP_EQ:  fprintf (prog_file, "je  :"); ret_val = 1; break;
    }

    if (ret_val == 1)
    {
        if (prog_data->label > prog_data->cond_depth) prog_data->cond_depth = prog_data->label;
        fprintf (prog_file, "%d\r\n", prog_data->cond_depth);
        return ret_val;
    }
    return 0;
}
