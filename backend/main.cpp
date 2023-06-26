#include "backend.h"

int main ()
{
    log_init ("./logs/back_log.html");
    char* tree_buffer = read_of_file ("./tree_stand/std_format.ast");

    tree_node_t* prog_tree =  build_of_tree (tree_buffer);
    graph_dump  (prog_tree);

//WRITING_OF_ASM_CODE-----------------------------------------------------------------------------

    FILE* prog_file = fopen ("./backend/prog_files/prog_code.asm", "w");
    MY_ASSERT (prog_file != NULL);
    write_asm_code (prog_tree, prog_file);

//DTOR_OF_PROG_INFORM-----------------------------------------------------------------------------------------------------------------------------------

    fclose      (prog_file);
    free        (tree_buffer);
    tree_delete (prog_tree);
    log_distr   ();
    return 0;
}
