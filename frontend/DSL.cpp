#include "DSL.h"

tree_node_t* make_gart_node (tree_node_t* l_child, tree_node_t* r_child)
{
    return tree_new_op_node (OP_GART_N, l_child, r_child);
}
