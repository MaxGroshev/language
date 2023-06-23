//#pragma once
#include "backend.h"
#include "../frontend/frontend.h" // make lib with mutual functions: strtod; l_strncomp

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

    if (l_strncomp (tree_buffer + pos_in_buf, ";", strlen (";"), STR_SKIP_SPACE, &pos_in_buf))
    {
        return tree_new_op_node (OP_GART_N);
    }
    if (l_strncomp   (tree_buffer + pos_in_buf, "var", strlen ("var"), STR_SKIP_SPACE, &pos_in_buf))
    {
        tree_node_t* var_node = tree_new_op_node (TYPE_VAR);
        var_node->name[0]  = 'v'; // for instance var1 change later
        return var_node;
    }
    if (l_strncomp   (tree_buffer + pos_in_buf, "nil", strlen ("nil"), STR_SKIP_SPACE, &pos_in_buf))
    {
        return NULL;
    }
    if (l_strncomp   (tree_buffer + pos_in_buf, "=", strlen ("="), STR_SKIP_SPACE, &pos_in_buf))
    {
        return tree_new_op_node (OP_EQ);
    }
    if (l_strncomp   (tree_buffer + pos_in_buf, "#meow", strlen ("#meow"), STR_SKIP_SPACE, &pos_in_buf))
    {
        tree_node_t* func_node = tree_new_op_node (TYPE_FUNC);
        func_node->name[0]  = 'm';
        return func_node;
    }
    if (tree_buffer[pos_in_buf] >= '0' && tree_buffer[pos_in_buf] <= '9')
    {
        printf ("want num");
        return tree_new_num_node (my_strtod (tree_buffer + pos_in_buf, &pos_in_buf));
    }

    if (l_strncomp (tree_buffer + pos_in_buf, "{", strlen ("{"), STR_SKIP_SPACE, &pos_in_buf))
    {
        tree_node_t* root   = build_of_tree (tree_buffer);
        tree_node_t* l_node = build_of_tree (tree_buffer);
        tree_node_t* r_node = build_of_tree (tree_buffer);
        if (l_node != NULL) tree_link_l (root, l_node); // make if (l_node != NULL)
        if (r_node != NULL) tree_link_r (root, r_node);
        printf ("Why\n");

        if (l_strncomp (tree_buffer + pos_in_buf, "}", strlen ("}"), STR_SKIP_SPACE, &pos_in_buf))
        {
            return root;
        }
    }
    return NULL;
}

//=========================================================================================================================================================
