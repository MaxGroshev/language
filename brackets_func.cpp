#include "brackets_func.h"

double tree_eval (tree_node_t* tree_node)
{
    MY_ASSERT (tree_node != NULL)

    if (tree_node->node_type == TYPE_NUM)
    {
        return tree_node->value;
    }

    switch (tree_node->node_type)
    {
        case OP_ADD:
            return tree_eval (tree_node->left) + tree_eval (tree_node->right);

        case OP_SUB:
            return tree_eval (tree_node->left) - tree_eval (tree_node->right);

        case OP_MUL:
            return tree_eval (tree_node->left) * tree_eval (tree_node->right);

        case OP_DIV:
        {
            double tmp_val = tree_eval (tree_node->right);
            if (tmp_val == 0)
            {
                write_tree_logs (T_DIVISION_BY_ZERO);
                return tmp_val;
            }
            return tree_eval (tree_node->left) / tree_eval (tree_node->right);
        }
        case OP_POW:
        {
            fprintf (stderr, "was made int power\n");
            write_extra_logs (T_INT_POWERED);
            double bas = tree_eval (tree_node->left);
            int deg = tree_eval (tree_node->right);
            if ((bas == 1) || (deg == 0)) return 1;
            else if (deg > 0)
            {
                for (int i = 1; i < deg; i++)
                {
                    bas = bas * bas;
                }
            }
            else if (deg < 0)
            {
                printf ("%lg\n", bas);
                double div_bas = 1 / bas;
                printf ("%lg\n", deg);
                for (int i = 0; i > deg + 1; i--)
                {
                    div_bas = div_bas / bas;
                    printf ("%lg\n", div_bas);
                }
                return div_bas;
            }
            return bas;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------------

int write_brackets (FILE* br_write, tree_node_t* tree_node)
{
    MY_ASSERT (tree_node != NULL && br_write != NULL)

    fprintf (br_write, "(");
    if (tree_node->left != NULL)
    {
        write_brackets (br_write, tree_node->left);
    }

    if      (tree_node->node_type == TYPE_NUM) fprintf (br_write, "%d", tree_node->value);
    else if (tree_node->node_type == CONST_EXP)fprintf (br_write, "np.exp");
    else if (tree_node->node_type == TYPE_VAR) fprintf (br_write, "%c", tree_node->value);
    else if (tree_node->node_type == OP_ADD)   fprintf (br_write, "+");
    else if (tree_node->node_type == OP_SUB)   fprintf (br_write, "-");
    else if (tree_node->node_type == OP_MUL)   fprintf (br_write, "*");
    else if (tree_node->node_type == OP_DIV)   fprintf (br_write, "/");
    else if (tree_node->node_type == OP_POW)   fprintf (br_write, "**");
    else if (tree_node->node_type == OP_LN)    fprintf (br_write, "np.log");
    else if (tree_node->node_type == OP_LOG)   fprintf (br_write, "np.log");
    else if (tree_node->node_type == OP_SIN)   fprintf (br_write, "np.sin");
    else if (tree_node->node_type == OP_COS)   fprintf (br_write, "np.cos");
    else if (tree_node->node_type == OP_TG)    fprintf (br_write, "np.tg");
    else if (tree_node->node_type == OP_CTG)   fprintf (br_write, "np.ctg");

    if (tree_node->right != NULL)
    {
        write_brackets (br_write, tree_node->right);
    }

    fprintf (br_write, ")");
    return 0;
}

tree_node_t* read_brackets (FILE* br_read, tree_node_t* tmp_node)
{
    MY_ASSERT (br_read != NULL)

    char buf [40];
    char br_elem;
    int  ret_val = fscanf (br_read, " %c", &br_elem);

    if (br_elem == '(')
    {
        int value = 0;
        int ret_val = fscanf (br_read, " %d", &value);
        if (ret_val != 0)
        {
            tree_node_t* tmp_node = tree_new_num_node (value);
            fscanf (br_read, " %c", &br_elem);
            if (br_elem == ')') return tmp_node;
        }

        tree_node_t* tmp_node = read_brackets (br_read);

        fscanf (br_read, " %s", buf);
        if (strcmp (buf, "(") == 0)  fscanf (br_read, " %s", buf);

        printf ("%s\n", buf);
        if (strcmp (buf, "+") == 0)
        {
            tree_node_t* tmp_parent  = tree_new_op_node (OP_ADD, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);
            return tmp_parent;
        }
        else if (strcmp (buf, "-") == 0)
        {
            tree_node_t* tmp_parent  = tree_new_op_node (OP_SUB, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);
            return tmp_parent;
        }
        else if (strcmp (buf, "*") == 0)
        {
            tree_node_t* tmp_parent  = tree_new_op_node (OP_MUL, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);
            return tmp_parent;
        }
        else if (strcmp (buf, "/") == 0)
        {
            tree_node_t* tmp_parent  = tree_new_op_node (OP_DIV, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);
            return tmp_parent;
        }
        else if (strcmp (buf, "^") == 0)
        {
            tree_node_t* tmp_parent  = tree_new_op_node (OP_POW, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);
            return tmp_parent;
        }
        else if (strcmp (buf, "ctg") == 0)
        {
            printf ("hey\n");
            tree_node_t* tmp_parent  = tree_new_op_node (OP_CTG, tmp_node);
            tree_node_t* tmp_node    = read_brackets (br_read);
            tree_link_r (tmp_parent, tmp_node);

            return tmp_parent;
        }
    }
    return NULL;
}
