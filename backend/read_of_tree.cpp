#include "backend.h"
 // make lib with mutual functions: strtod; l_strncomp

char* read_of_file (const char* file_dir)
{
    FILE* tree_file = fopen (file_dir, "r");
    MY_ASSERT (tree_file != NULL);

    struct stat file_data = {};
    stat (file_dir, &file_data);
    char* tree_buffer = (char*) calloc (file_data.st_size + 1, sizeof (char));
    MY_ASSERT (tree_buffer != NULL);

    fread  (tree_buffer, sizeof (char), file_data.st_size, tree_file);
    fclose (tree_file);
    return tree_buffer;
}

//=========================================================================================================================================================

tree_node_t* build_of_tree (char* tree_buffer)
{
    static int pos_in_buf = 0;

    if (IS_THIS_OPER ("if"))     return tree_new_op_node (OP_IF);
    if (IS_THIS_OPER ("return")) return tree_new_op_node (OP_RETURN);

    if (IS_THIS_OPER (";"))   return tree_new_op_node (OP_GART_N);
    if (IS_THIS_OPER ("nil")) return NULL;
    if (IS_THIS_OPER ("=="))  return tree_new_op_node (OP_COMP_EQ);
    if (IS_THIS_OPER ("!="))  return tree_new_op_node (OP_N_COMP_EQ);
    if (IS_THIS_OPER (">="))  return tree_new_op_node (OP_ABOVE_EQ);
    if (IS_THIS_OPER ("<="))  return tree_new_op_node (OP_LESS_EQ);
    if (IS_THIS_OPER (">"))   return tree_new_op_node (OP_ABOVE);
    if (IS_THIS_OPER ("<"))   return tree_new_op_node (OP_LESS);
    if (IS_THIS_OPER ("="))   return tree_new_op_node (OP_EQ);
    if (IS_THIS_OPER ("+"))   return tree_new_op_node (OP_ADD);
    if (IS_THIS_OPER ("-"))
    {
        if (tree_buffer[pos_in_buf] >= '0' && tree_buffer[pos_in_buf] <= '9')
        {
            return tree_new_num_node (-(my_strtod (tree_buffer + pos_in_buf, &pos_in_buf)));
        }
        return tree_new_op_node (OP_SUB);
    }
    if (IS_THIS_OPER ("*"))   return tree_new_op_node (OP_MUL);
    if (IS_THIS_OPER ("/"))   return tree_new_op_node (OP_DIV);
    if (IS_THIS_OPER ("^"))   return tree_new_op_node (OP_POW);
    if (IS_THIS_OPER (","))   return tree_new_op_node (OP_COMMA);

    if (tree_buffer[pos_in_buf] >= '0' && tree_buffer[pos_in_buf] <= '9')
    {
        return tree_new_num_node (my_strtod (tree_buffer + pos_in_buf, &pos_in_buf));
    }
    if (l_strncomp   (tree_buffer + pos_in_buf, "var", strlen ("var"), STR_SKIP_SPACE, &pos_in_buf))
    {
        tree_node_t* var_node = tree_new_op_node (TYPE_VAR);

        int num_of_var = my_strtod (tree_buffer + pos_in_buf, &pos_in_buf);
        char buf[BUF_OF_64_ELEM];
        sprintf (buf, "var%d", num_of_var);
        strncpy (var_node->name, buf, BUF_OF_64_ELEM);
        var_node->num_of_var = num_of_var;

        return var_node;
    }
    if (IS_THIS_OPER ("#"))
    {
        tree_node_t* func_node = tree_new_op_node (TYPE_FUNC);

        if      (IS_THIS_OPER ("meow"))    strncpy (func_node->name, "meow", strlen ("meow"));
        else if (IS_THIS_OPER ("print"))   func_node->value    =  LIB_PRINT;
        else if (IS_THIS_OPER ("writeln")) func_node->value    =  LIB_WRITELN;
        else if (IS_THIS_OPER ("sqrt"))     func_node->value    =  LIB_SQR;

        return func_node;
    }
    if (IS_THIS_OPER ("{"))
    {
        tree_node_t* root   = build_of_tree (tree_buffer);
        tree_node_t* l_node = build_of_tree (tree_buffer);
        tree_node_t* r_node = build_of_tree (tree_buffer);
        if (l_node != NULL) tree_link_l (root, l_node);
        if (r_node != NULL) tree_link_r (root, r_node);

        if (IS_THIS_OPER ("}"))
        {
            return root;
        }
    }

    return NULL;
}

//=========================================================================================================================================================


