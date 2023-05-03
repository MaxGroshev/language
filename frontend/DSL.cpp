#include "DSL.h"

tree_node_t* make_gart_node ()
{
    return tree_new_op_node (OP_GART_N);
}

tree_node_t* New_num (tree_data_type value)
{
    return tree_new_num_node (value);
}

tree_node_t* New_const (tree_data_type value)
{
    return tree_new_const_node (value);
}

tree_node_t* New_var (int node_type, char var_name)
{
    return tree_new_var_node (node_type, var_name);
}

tree_node_t* Add (tree_node_t* left_node, tree_node_t* right_node)
{
    return tree_new_op_node (OP_ADD, left_node, right_node);
}

tree_node_t* Sub (tree_node_t* left_node, tree_node_t* right_node)
{
    return tree_new_op_node (OP_SUB, left_node, right_node);
}

tree_node_t* Mul (tree_node_t* left_node, tree_node_t* right_node)
{
    return tree_new_op_node (OP_MUL, left_node, right_node);
}

tree_node_t* Div (tree_node_t* left_node, tree_node_t* right_node)
{
    return tree_new_op_node (OP_DIV, left_node, right_node);
}

tree_node_t* Pow (tree_node_t* left_node, tree_node_t* right_node)
{
    return tree_new_op_node (OP_POW, left_node, right_node);
}

tree_node_t* Ln (tree_node_t* right_node)
{
    return tree_new_op_node (OP_LN, NULL, right_node);
}

tree_node_t* Cos (tree_node_t* right_node)
{
    return tree_new_op_node (OP_COS, NULL, right_node);
}

tree_node_t* Sin (tree_node_t* right_node)
{
    return tree_new_op_node (OP_SIN, NULL, right_node);
}

tree_node_t* Tg (tree_node_t* right_node)
{
    return tree_new_op_node (OP_SIN, NULL, right_node);
}

tree_node_t* Ctg (tree_node_t* right_node)
{
    return tree_new_op_node (OP_SIN, NULL, right_node);
}

//------------------------------------------------------------------------------------------------------------------------------------------

int is_arithm_op (tree_node_t* tree_node)
{
    if (tree_node == NULL) return 0; //?!
    else if (tree_node->node_type == OP_ADD || tree_node->node_type == OP_SUB || tree_node->node_type == OP_MUL ||
        tree_node->node_type == OP_DIV || tree_node->node_type == OP_POW) return 1;
    return 0;
}
