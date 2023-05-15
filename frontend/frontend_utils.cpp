#include "frontend.h"

int frontend_utils (const char* file_dir)
{
    char* buffer = read_file (file_dir);

//INIT_OF_STRUCT_WITH_LEXEMS-------------------------------------------------------------------------------

    prog_data_t prog_stat = {};
    prog_stat_init (&prog_stat);
    lex_stat_t lex_stat = {};
    lexems_init (&lex_stat);

//START_OF_LEXICAL_ANAYSIS---------------------------------------------------------------------------

    lexical_analysis (buffer, &lex_stat, &prog_stat);

    FILE* array = fopen ("./frontend/debug", "w");
    for (int i = 0; i < lex_stat.lex_size; i++)
    {
        fprintf (array, "%d\n", lex_stat.lexems[i].node_type);
    }
    fclose (array);

//START_OF_REC_DESCENT------------------------------------------------------------------------------------

    tree_node_t* prog_tree = rec_descent (&lex_stat, &prog_stat);
    graph_dump  (prog_tree);

//DTOR_OF_LEXEMS_STRUCT------------------------------------------------------------------------------------

    lex_dtor (&lex_stat);
    prog_data_dtor (buffer, &prog_stat);

    return 0;
}
