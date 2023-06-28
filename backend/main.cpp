#include "backend.h"

int main ()
{
    log_init ("./logs/back_log.html");
    char* tree_buffer = read_of_file ("./tree_stand/std_format.ast");

    prog_data_t prog_data = {};

    tree_node_t* prog_tree =  build_of_tree (tree_buffer, &prog_data);
    graph_dump  (prog_tree);

//WRITING_OF_ASM_CODE-----------------------------------------------------------------------------


    FILE* prog_file = fopen ("./backend/prog_files/prog_code.asm", "w");
    MY_ASSERT (prog_file != NULL);
    fprintf (prog_file, "call :0\r\nhlt\r\n"); //improve (at least remove)

    write_asm_code (prog_tree, prog_file, &prog_data);

//DTOR_OF_PROG_INFORM-----------------------------------------------------------------------------------------------------------------------------------

    fclose      (prog_file);
    free        (tree_buffer);
    tree_delete (prog_tree);
    log_distr   ();
    return 0;
}
