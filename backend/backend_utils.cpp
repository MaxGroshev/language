#include "backend.h"

int backend_utils (const char* tree_dir)
{
    char* tree_buffer = read_of_file (tree_dir); // change to read_file when different main

    tree_node_t* prog_tree =  build_of_tree (tree_buffer);
    graph_dump  (prog_tree);

//DTOR OF PROG_INFORM-----------------------------------------------------------------------------------------------------------------------------------
    free        (tree_buffer);
    tree_delete (prog_tree);
    return 0;
}
