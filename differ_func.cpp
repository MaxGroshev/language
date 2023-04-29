#include "language.h"

tree_node_t* dif_node (const tree_node_t* tree_node, char dif_var)
{
    switch (tree_node->node_type)
    {
        case TYPE_NUM:
            return New_num (0);

        case CONST_EXP:
            return New_const (0);

        case TYPE_VAR:
            if (dif_var == tree_node->value) return New_num (1);
            return New_num (0);

        case OP_ADD:
            return Add (Dif_l, Dif_r);

        case OP_SUB:
            return Sub (Dif_l, Dif_r);

        case OP_MUL:
        {
            return Add (Mul (Dif_l, Copy_r), Mul (Dif_r, Copy_l));
        }
        case OP_DIV:
        {
            tree_node_t* numer = Sub (Mul (Dif_l, Copy_r), Mul (Dif_r, Copy_l));
            tree_node_t* denom = Pow (Copy_r, New_num (2));

            return Div (numer, denom);
        }
        case OP_LN:
        {
            return Mul (Div (New_num (1), Copy_r), Dif_r);
        }
        case OP_LOG:
        {
            tree_node_t* denom = Mul (Copy_r, Ln (Copy_l));
            return Mul (Div (New_num (1), denom), Dif_r);
        }
        case OP_SIN:
        {
            return Mul (Cos (Copy_r), Dif_r);
        }
        case OP_COS:
        {
            tree_node_t* ex_der = Mul (New_num (-1), Sin (Copy_r));
            return Mul (ex_der, Dif_r);
        }
        case OP_TG:
        {
            tree_node_t* ex_der = Div (New_num (1), Pow (Cos (Copy_r), New_num (2)));
            return Mul (ex_der, Dif_r);
        }
        case OP_CTG:
        {
            tree_node_t* ex_der = Mul (New_num (-1), Div (New_num (1), Pow (Sin (Copy_r), New_num (2))));
            return Mul (ex_der, Dif_r);
        }
        case OP_POW:
        {
            if (tree_node->right->node_type == TYPE_NUM)
            {
                tree_node_t* power = Pow (Copy_l, Sub (Copy_r, New_num (1)));
                return Mul (Copy_r, Mul (Dif_l, power));
            }
            else if (tree_node->left->node_type == CONST_EXP)
            {
                return Mul (Pow (New_const (CONST_EXP), Copy_r), Dif_r);
            }
            else
            {
                tree_node_t* exp    = Pow (New_const (CONST_EXP), Mul (Copy_r, Ln (Copy_l)));
                tree_node_t* ln_dif = Mul (Div (New_num (1), Copy_l), Dif_l );
                tree_node_t* in_der = Add (Mul (Dif_r, Ln (Copy_l)), Mul (ln_dif, Copy_r));
                return Mul (exp, in_der);
            }
        }
    }
    write_tree_logs (T_UNSUPPORTED_OPER);
    return NULL;
}

tree_node_t* copy_node (const tree_node_t* tree_node)
{
    MY_ASSERT (tree_node != NULL)
    switch (tree_node->node_type)
    {
        case TYPE_NUM:
            return New_num (tree_node->value);

        case CONST_EXP:
            return New_const (tree_node->value);

        case TYPE_VAR:
            return New_var (tree_node->node_type, tree_node->value);

        case OP_ADD:
            return Add (Copy_l, Copy_r);

        case OP_SUB:
            return Sub (Copy_l, Copy_r);

        case OP_MUL:
            return Mul (Copy_l, Copy_r);

        case OP_DIV:
            return Div (Copy_l, Copy_r);

        case OP_POW:
            return Pow (Copy_l, Copy_r);

        case OP_LN:
            return Ln  (Copy_r);

        case OP_SIN:
            return Sin (Copy_r);

        case OP_COS:
            return Cos (Copy_r);

        case OP_TG:
            return Tg (Copy_r);

        case OP_CTG:
            return Ctg (Copy_r);
    }
    return NULL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

tree_node_t* simpl_node (tree_node_t* tree_node, char dif_var)
{
    MY_ASSERT (tree_node != NULL)
    int change_flag = 1;
    for (int i = 1; change_flag != 0; i++)
    {
        write_extra_logs ("\n<font color = #8DB6CD size=6>It is my %d attempt to simplify in %s\n</font>", i, __FUNCTION__ );
        change_flag = 0;
        tree_node = simpl_func (tree_node, dif_var, &change_flag);
        graph_dump      (tree_node);
    }
    return tree_node;
}

tree_node_t* simpl_func (tree_node_t* tree_node, char dif_var, int* change_flag)
{
    static int rec_level   = 0;
    if (is_arithm_op (tree_node))
    {
        if (tree_node->right->node_type == TYPE_NUM && tree_node->left->node_type == TYPE_NUM)
        {
            *change_flag = 1;
            int value = tree_eval (tree_node);
            tree_delete (tree_node);
            return New_num (value);
        }

        if (tree_node->node_type == OP_MUL)
        {
            if ((tree_node->left->node_type == TYPE_NUM  && tree_node->left->value ==  0) ||
                ( tree_node->right->node_type == TYPE_NUM && tree_node->right->value == 0))
            {
                *change_flag = 1;
                tree_delete (tree_node);
                return New_num (0);
            }
            if (tree_node->left->node_type == TYPE_NUM  && tree_node->left->value == 1)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = Copy_r;
                tree_delete (tree_node);
                return tmp_node;
            }
            if (tree_node->right->node_type == TYPE_NUM  && tree_node->right->value == 1)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = Copy_l;
                tree_delete (tree_node);
                return tmp_node;
            }
            if ((tree_node->left->node_type == TYPE_VAR && tree_node->right->node_type == TYPE_VAR) && tree_node->left->value == tree_node->right->value)
            {
                tree_node_t* tmp_node = Pow (New_var (TYPE_VAR, tree_node->right->value), New_num (2));
                tree_delete (tree_node);
                return tmp_node;
            }
        }

        if (tree_node->node_type == OP_ADD)
        {
            if (is_arithm_op (tree_node->right))
            {
                if (tree_node->right->left->value < 0)
                {
                    *change_flag = 1;
                    tree_node_t* tmp_num_node   = New_num (-tree_node->right->left->value);
                    tree_node_t* tmp_func_node1 = copy_node (tree_node->right->right);
                    tree_node_t* tmp_r_side     = tree_new_op_node (tree_node->right->node_type, tmp_num_node, tmp_func_node1);
                    tree_delete (tree_node->right);
                    tree_node->right = NULL;

                    tree_node_t* tmp_func_node2 = copy_node (tree_node->left);
                    tree_delete (tree_node);
                    return tree_new_op_node (OP_SUB, tmp_func_node2, tmp_r_side);
                }
                if (tree_node->right->right->value < 0) // was seg fault
                {
                    *change_flag = 1;
                    tree_node_t* tmp_num_node   = New_num   (-tree_node->right->right->value);
                    tree_node_t* tmp_func_node1 = copy_node (tree_node->right->left);
                    tree_node_t* tmp_r_side     = tree_new_op_node (tree_node->right->node_type, tmp_num_node, tmp_func_node1);
                    tree_delete (tree_node->right);
                    tree_node->right = NULL;

                    tree_node_t* tmp_func_node2 = copy_node (tree_node->left);
                    tree_delete (tree_node);
                    return tree_new_op_node (OP_SUB, tmp_func_node2, tmp_r_side);
                }
            }
            if (tree_node->left->node_type == TYPE_NUM && tree_node->left->value == 0)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = copy_node (tree_node->right);
                tree_delete (tree_node);
                return (tmp_node);
            }
            if (tree_node->right->node_type == TYPE_NUM && tree_node->right->value == 0)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = copy_node (tree_node->left);
                tree_delete (tree_node);
                return (tmp_node);
            }
        }

        if (tree_node->node_type == OP_SUB)
        {
            if (tree_node->left->node_type == TYPE_NUM && tree_node->left->value == 0)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = copy_node (tree_node->right);
                tree_delete (tree_node);
                return (tmp_node);
            }
            else if (tree_node->right->node_type == TYPE_NUM && tree_node->right->value == 0)
            {
                *change_flag = 1;
                tree_node_t* tmp_node = copy_node (tree_node->left);
                tree_delete (tree_node);
                return (tmp_node);
            }
        }
    }

    if (tree_node->left != NULL)
    {
        rec_level++;
        tree_node_t* ret_node = simpl_func (tree_node->left, dif_var, change_flag);
        rec_level--;
        if (tree_node->left != ret_node)
        {
            tree_node->left = NULL;
            tree_link_l (tree_node, ret_node);
        }
    }
    if (tree_node->right != NULL)
    {
        rec_level++;
        tree_node_t* ret_node = simpl_func (tree_node->right, dif_var, change_flag);
        rec_level--;
        if (tree_node->right != ret_node)
        {
            tree_node->right = NULL;
            tree_link_r (tree_node, ret_node);
        }
    }

    return tree_node;
}
