#include "frontend.h"

int main ()
{
    log_init ("./logs/front_log.html");
    char* buffer = read_file ("./lang.my");

//INIT_OF_STRUCT_WITH_LEXEMS-------------------------------------------------------------------------------

    prog_data_t prog_stat = {};
    prog_stat_init (&prog_stat);
    lex_stat_t lex_stat = {};
    lexems_init (&lex_stat);

//START_OF_LEXICAL_ANAYSIS---------------------------------------------------------------------------

    lexical_analysis (buffer, &lex_stat, &prog_stat);

    FILE* array = fopen ("./frontend/debug", "w");
    MY_ASSERT (array != NULL)
    for (int i = 0; i < lex_stat.lex_size; i++)
    {
        fprintf (array, "%d\n", lex_stat.lexems[i].node_type);
    }
    fclose (array);

//START_OF_REC_DESCENT------------------------------------------------------------------------------------

    tree_node_t* prog_tree = rec_descent (&lex_stat, &prog_stat);
    MY_ASSERT   (prog_tree != NULL);
    graph_dump  (prog_tree);

//START_OF_TREE_BUILD

    print_standart (&prog_stat, prog_tree);

//DTOR_OF_LEXEMS_STRUCT------------------------------------------------------------------------------------

    lex_dtor       (&lex_stat); //Will be improved by adding tree_file
    prog_data_dtor (buffer, &prog_stat);
    log_distr ();

    return 0;
}
