#include "backend.h"

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

tree_node_t* build_of_tree (char* tree_buffer, prog_data_t* prog_data)
{
    tree_node_t* prog_tree = read_oper (tree_buffer, prog_data);
    MY_ASSERT (prog_tree != NULL)

    if (prog_data->meow_flag != 1)
    {
        printf ("\033[91mSyntax error. NO ""meow"" definition. Poltorashka feels sorry\033[0m\n");
        exit (1);
    }

    return prog_tree;
}

tree_node_t* read_oper (char* tree_buffer, prog_data_t* prog_data)
{
    static int pos_in_buf = 0;

    if (IS_THIS_OPER ("if"))     return tree_new_op_node (OP_IF);
    if (IS_THIS_OPER ("while"))  return tree_new_op_node (OP_WHILE);
    if (IS_THIS_OPER ("return")) return tree_new_op_node (OP_RETURN);

    if (IS_THIS_OPER (";"))      return tree_new_op_node (OP_GART_N);
    if (IS_THIS_OPER ("nil"))    return NULL;
    if (IS_THIS_OPER ("=="))     return tree_new_op_node (OP_COMP_EQ);
    if (IS_THIS_OPER ("!="))     return tree_new_op_node (OP_N_COMP_EQ);
    if (IS_THIS_OPER (">="))     return tree_new_op_node (OP_ABOVE_EQ);
    if (IS_THIS_OPER ("<="))     return tree_new_op_node (OP_LESS_EQ);
    if (IS_THIS_OPER (">"))      return tree_new_op_node (OP_ABOVE);
    if (IS_THIS_OPER ("<"))      return tree_new_op_node (OP_LESS);
    if (IS_THIS_OPER ("="))      return tree_new_op_node (OP_EQ);
    if (IS_THIS_OPER ("+"))      return tree_new_op_node (OP_ADD);
    if (IS_THIS_OPER ("*"))      return tree_new_op_node (OP_MUL);
    if (IS_THIS_OPER ("/"))      return tree_new_op_node (OP_DIV);
    if (IS_THIS_OPER ("^"))      return tree_new_op_node (OP_POW);
    if (IS_THIS_OPER (","))      return tree_new_op_node (OP_COMMA);
    if (IS_THIS_OPER ("-"))
    {
        if (tree_buffer[pos_in_buf] >= '0' && tree_buffer[pos_in_buf] <= '9')
        {
            return tree_new_num_node (-(my_strtod (tree_buffer + pos_in_buf, &pos_in_buf)));
        }
        return tree_new_op_node (OP_SUB);
    }
    if (tree_buffer[pos_in_buf] >= '0' && tree_buffer[pos_in_buf] <= '9')
    {
        return tree_new_num_node (my_strtod (tree_buffer + pos_in_buf, &pos_in_buf));
    }
    if (IS_THIS_OPER ("var")) return read_var  (tree_buffer, &pos_in_buf, prog_data);
    if (IS_THIS_OPER ("#"))   return read_func (tree_buffer, &pos_in_buf, prog_data);

    if (IS_THIS_OPER ("{"))
    {
        tree_node_t* root   = read_oper (tree_buffer, prog_data);
        tree_node_t* l_node = read_oper (tree_buffer, prog_data);
        tree_node_t* r_node = read_oper (tree_buffer, prog_data);
        if (l_node != NULL) tree_link_l (root, l_node);
        if (r_node != NULL) tree_link_r (root, r_node);

        if (IS_THIS_OPER ("}"))
        {
            return root;
        }
    }

    return NULL;
}

//=======================================READING_FUNCTIONS==================================================================================================================

tree_node_t* read_func (char* tree_buffer, int* pos_in_buf ,prog_data_t* prog_data)
{
    tree_node_t* func_node = tree_new_op_node (TYPE_FUNC);
    int decl = -1;

    if (IS_THIS_FUNC ("meow"))
    {
        strncpy (func_node->name, "meow", strlen ("meow"));
        func_node->num_of_var_func = 0;
        prog_data->meow_flag       = 1;
        func_node->decloration = L_DECL;
        return func_node;
    }
    else if (IS_THIS_FUNC ("print"))   func_node->value    =  LIB_PRINT;
    else if (IS_THIS_FUNC ("writeln")) func_node->value    =  LIB_WRITELN;
    else if (IS_THIS_FUNC ("sqrt"))    func_node->value    =  LIB_SQR;
    else if (IS_THIS_FUNC ("dec_func")) decl = L_DECL;
    else if (IS_THIS_FUNC ("func"))     decl = L_MENTION;

    if (decl == L_DECL || decl == L_MENTION)
    {
        int num_of_func = my_strtod (tree_buffer + *pos_in_buf, pos_in_buf);
        char buf[BUF_OF_64_ELEM];
        sprintf (buf, "func%d", num_of_func);
        strncpy (func_node->name, buf, BUF_OF_64_ELEM);
        func_node->num_of_var_func = num_of_func;

        if (decl == L_DECL)
        {
            func_node->decloration = L_DECL;
            prog_data->label++; //just to start if jmp from func calls
        }
        else if (decl == L_MENTION) func_node->decloration = L_MENTION;
    }

    return func_node;
}

tree_node_t* read_var (char* tree_buffer, int* pos_in_buf, prog_data_t* prog_data)
{
    tree_node_t* var_node = tree_new_op_node (TYPE_VAR);

    int num_of_var = my_strtod (tree_buffer + *pos_in_buf, pos_in_buf);
    char buf[BUF_OF_64_ELEM];
    sprintf (buf, "var%d", num_of_var);
    strncpy (var_node->name, buf, BUF_OF_64_ELEM);
    var_node->num_of_var_func = num_of_var;

    return var_node;
}
