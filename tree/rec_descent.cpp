#include "tree.h"

static int pos_in_file = 0;
tree_node_t* rec_descent (const char* file_dir)
{
    char* buffer = read_file (file_dir);
    tree_node_t* ret_node = get_end (buffer);
    free (buffer);
    return ret_node;
}

//------------------------------------REC_DESCENT_REALIZATION--------------------------------------------------------------

tree_node_t* get_end (const char* buffer)
{
    tree_node_t* tree_node = get_sign (buffer);

    if (buffer[pos_in_file] != '\r' && buffer[pos_in_file] != '\n') syntax_error (S_UNREC_SYNTAX_ERROR, buffer, CUR_POS_IN_PROG);
    return tree_node;
}

tree_node_t* get_sign (const char* buffer)
{
    int is_neg_val = 0;
    if (buffer[pos_in_file] == '-')
    {
        is_neg_val = 1;
        pos_in_file++;
    }
    tree_node_t* r_node = get_pm_sign (buffer);
    if (is_neg_val == 1) return tree_new_op_node (OP_MUL, tree_new_num_node (-1), r_node);
    return r_node;
}

tree_node_t* get_pm_sign (const char* buffer)
{
    tree_node_t* l_node = get_md_sign (buffer);

    while (buffer[pos_in_file] == '+' || buffer[pos_in_file] == '-')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_md_sign (buffer);
        if   (op == '+') l_node = tree_new_op_node (OP_ADD, l_node, r_node);
        else             l_node = tree_new_op_node (OP_SUB, l_node, r_node);
    }
    return l_node;
}

tree_node_t* get_md_sign (const char* buffer)
{
    tree_node_t* l_node = get_deg (buffer);
    while (buffer[pos_in_file] == '*' || buffer[pos_in_file] == '/')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_deg (buffer);
        if      (op == '*')  l_node = tree_new_op_node (OP_MUL, l_node, r_node);
        else if (op == '/')  l_node = tree_new_op_node (OP_DIV, l_node, r_node);
        else    syntax_error (S_NO_MUL_OR_DIV_OP, buffer, CUR_POS_IN_PROG);
    }
    return l_node;
}

tree_node_t* get_deg (const char* buffer)
{
    tree_node_t* l_node = get_func(buffer);
    while (buffer[pos_in_file] == '^')
    {
        int op = buffer[pos_in_file];
        pos_in_file++;
        tree_node_t* r_node = get_func (buffer);
        if      (op == '^')  l_node = tree_new_op_node (OP_POW, l_node, r_node);// make error
        else    syntax_error (S_NO_MUL_OR_DIV_OP, buffer, CUR_POS_IN_PROG);
    }
    return l_node;
}

tree_node_t* get_func (const char* buffer)
{
    const char* pos_in_func = NULL;
    if (strncomp (buffer + pos_in_file, "sin", strlen ("sin")))
    {
        pos_in_file += strlen ("sin") ;
        tree_node_t* parent = tree_new_op_node (OP_SIN, NULL, get_brac (buffer));
        return parent;
    }
    else if (strncomp (buffer + pos_in_file, "cos", strlen ("cos")))
    {
        pos_in_file += strlen ("cos") ;
        tree_node_t* parent = tree_new_op_node (OP_COS, NULL, get_brac (buffer));
        return parent;
    }
    else if (strncomp (buffer + pos_in_file, "ln", strlen ("ln")))
    {
        pos_in_file += strlen ("ln") ;
        tree_node_t* parent = tree_new_op_node (OP_LN, NULL, get_brac (buffer));
        return parent;
    }
    else if (strncomp (buffer + pos_in_file, "tg", strlen ("tg")))
    {
        pos_in_file += strlen ("tg") ;
        tree_node_t* parent = tree_new_op_node (OP_TG, NULL, get_brac (buffer));
        return parent;
    }
    else if (strncomp (buffer + pos_in_file, "ctg", strlen ("ctg")))
    {
        pos_in_file += strlen ("ctg") ;
        tree_node_t* parent = tree_new_op_node (OP_CTG, NULL, get_brac (buffer));
        return parent;
    }
    return get_brac (buffer);
}

tree_node_t* get_brac (const char* buffer)
{
    if ((buffer[pos_in_file] == '(') || (buffer[pos_in_file] == '['))
    {
        write_tree_logs (S_START_OF_BR_SEQ, NULL, CUR_POS_IN_PROG);
        pos_in_file++;
        tree_node_t* tree_node = get_sign (buffer);
        if ((buffer[pos_in_file] != ')') && (buffer[pos_in_file] != ']')) syntax_error (S_NO_CLOSED_BRACKETS, buffer, CUR_POS_IN_PROG);
        pos_in_file++;
        return tree_node;
    }
    else return get_ident (buffer);
}

tree_node_t* get_ident (const char* buffer) //add multisymbol vars
{
    if ((buffer[pos_in_file] >= 'a' && buffer[pos_in_file] <= 'z') && buffer[pos_in_file] != 'e')
    {
        char var = buffer[pos_in_file];
        pos_in_file++;
       // printf ("%c", buffer[pos_in_file]);
        if (buffer[pos_in_file] == '=')
        {
            pos_in_file++;
            printf ("%c", buffer[pos_in_file]);
            tree_node_t* r_value = get_sign (buffer);
            return tree_new_op_node (OP_EQ, tree_new_var_node (TYPE_VAR, var), r_value);
        }
        return tree_new_var_node (TYPE_VAR, var);
    }
    else return get_num (buffer);
}

tree_node_t* get_num (const char* buffer)
{
    int val = 0;
    int start_pos = pos_in_file;
    while (buffer[pos_in_file] >= '0' && buffer[pos_in_file] <= '9')
    {
        val = val * 10 + buffer[pos_in_file] - '0';
        pos_in_file++;
    }
    if (start_pos >= pos_in_file) syntax_error (S_NO_NUMBER, buffer, CUR_POS_IN_PROG);
    return tree_new_num_node (val);
}

//----------------------------------------------------------------------------------------------------------------------

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

void syntax_error (int num_of_error, const char* buffer, const char* file_name, const char* func_name, int num_of_line)
{
    fprintf (stderr, "\033[91mSyntaxERROR  \033[0m: FILE: %s, FUNCTION: %s, LINE: %d\n", file_name, func_name, num_of_line);
    switch (num_of_error)
    {
        case S_NO_CLOSED_BRACKETS:
            fprintf (stderr, "\033[91mNo closing bracket \033[0m: ");
            break;

        case S_NO_NUMBER:
            fprintf (stderr, "\033[91mWas expected num_value  \033[0m: ");
            break;

        case S_NO_MUL_OR_DIV_OP:
            fprintf (stderr, "\033[91mWas expected div or mul operation \033[0m: ");
            break;

        case S_UNREC_SYNTAX_ERROR:
            fprintf (stderr, "\033[91mName of syntax error wasn't detected. Check input. \033[0m: ");
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

int strncomp (const char* str1, const char* str2, size_t num_of_elem)
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
