#include "frontend.h"

int frontend_utils (const char* file_dir)
{
    char* buffer = read_file (file_dir);

    prog_data_t prog_stat = {};
    prog_stat_init (&prog_stat);

    lex_stat_t lex_stat = {};
    lexems_init (&lex_stat);

    lexical_analysis (buffer, &lex_stat, &prog_stat);
    printf ("%s \n", lex_stat.lexems[1].name);

    tree_node_t* prog_tree = rec_descent (&lex_stat, &prog_stat);
    graph_dump  (prog_tree);

    free (lex_stat.lexems);

    prog_data_dtor (buffer, &prog_stat);
}
