#include "backend.h"

int main ()
{
    log_init ("./logs/back_log.html");
    char* tree_buffer = read_of_file ("./tree_stand/std_format.ast");

    prog_data_t prog_data = {};

    tree_node_t* prog_tree =  build_of_tree (tree_buffer, &prog_data);
    graph_dump  (prog_tree);

//WRITING_OF_ASM_CODE-----------------------------------------------------------------------------

    write_asm_code (prog_tree, &prog_data);

//DTOR_OF_PROG_INFORM-----------------------------------------------------------------------------------------------------------------------------------

    free        (tree_buffer);
    tree_delete (prog_tree);
    log_distr   ();
    return 0;
}
