#include "tree.h"

tree_t* constructor (tree_t* pine)
{
    MY_ASSERT (pine!= NULL);

    pine->size = 0;
    pine->root = NULL;
    write_extra_logs (T_TREE_SUC_CREATED);

    return pine;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

tree_node_t* tree_new_num_node (tree_data_type value)
{
    tree_node_t* tmp_node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    MY_ASSERT   (tmp_node != NULL);

    tmp_node->value     = value;
    tmp_node->node_type = TYPE_NUM;
    write_tree_logs (T_NODE_SUC_CREATED, tmp_node);

    return  tmp_node;
}

tree_node_t* tree_new_const_node (tree_data_type value)
{
    tree_node_t* tmp_node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    MY_ASSERT   (tmp_node != NULL);

    tmp_node->value     = value;
    tmp_node->node_type = TYPE_NUM;
    if (tmp_node->value == CONST_EXP)
    {
        tmp_node->node_type = CONST_EXP;
    }
    write_tree_logs (T_NODE_SUC_CREATED, tmp_node);

    return  tmp_node;
}

tree_node_t* tree_new_var_node (int node_type, char var_name)
{
    tree_node_t* tmp_node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    MY_ASSERT   (tmp_node != NULL);

    tmp_node->node_type = TYPE_VAR;
    tmp_node->value     = var_name;
    write_tree_logs (T_NODE_SUC_CREATED, tmp_node);

    return  tmp_node;
}

tree_node_t* tree_new_op_node (int node_type, tree_node_t* l_child, tree_node_t* r_child)
{
    tree_node_t* tmp_node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    MY_ASSERT   (tmp_node != NULL);

    tmp_node->node_type = node_type;
    write_tree_logs (T_NODE_SUC_CREATED, tmp_node);

    if (l_child != NULL) tree_link_l (tmp_node, l_child);
    if (r_child != NULL) tree_link_r (tmp_node, r_child);
    MY_ASSERT   (tmp_node != NULL);

    return  tmp_node;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

tree_node_t* tree_link_l (tree_node_t* parent, tree_node_t* child)
{
    MY_ASSERT (parent != NULL && child != NULL);

    if (parent->node_type == TYPE_NUM) write_tree_logs (T_TYPE_NUM_HAS_CHILD, parent);
    else if (parent->left == NULL)
    {
        parent->left  = child;
        write_tree_logs (T_L_EDGE_SUC_CREATED, parent);
        return parent->left;
    }
    write_extra_logs (T_FAIL_OF_CREATING_EDGE);
    return NULL;
}

tree_node_t* tree_link_r (tree_node_t* parent, tree_node_t* child)
{
    MY_ASSERT (parent != NULL && child != NULL);
    if (parent->node_type == TYPE_NUM) write_tree_logs (T_TYPE_NUM_HAS_CHILD, parent);
    else if (parent->right == NULL)
    {

        parent->right = child;
        write_tree_logs (T_R_EDGE_SUC_CREATED, parent);
        return parent->right;
    }
    write_extra_logs (T_FAIL_OF_CREATING_EDGE);
    return NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

tree_node_t* tree_search (tree_node_t* tree_root, tree_node_t* node)
{
    printf ("here\n");
    MY_ASSERT (tree_root != NULL)
    if (tree_root->left != NULL)
    {
        printf ("here\n");
        if (tree_root->left == node || tree_root->right == node)
        {
            printf ("O---K\n");
            return tree_root;
        }
        MY_ASSERT (tree_root->left != NULL)
        tree_node_t* tree_root = tree_search (tree_root->left, node);
        printf ("returned\n");

        if (tree_root->left == node || tree_root->right == node)
        {
            return tree_root;
        }
    }
    if (tree_root->right != NULL)
    {
        if (tree_root->left == node || tree_root->right == node)
        {
            printf ("O---K\n");
            return tree_root;
        }
        tree_node_t* tree_root = tree_search (tree_root->right, node);
        if (tree_root->left == node || tree_root->right == node)
        {
            return tree_root;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

tree_node_t* tree_delete (tree_node_t* tree_node)
{
    if (tree_node == NULL) return NULL;
    if (tree_node->left != NULL)
    {
        tree_delete (tree_node->left);
    }
    if (tree_node->right != NULL)
    {
        tree_delete (tree_node->right);
    }
    free (tree_node);

    write_tree_logs (T_NODE_SUC_DELETED, tree_node);

    return NULL;
}




