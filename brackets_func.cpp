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
                printf ("%d\n", deg);
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
